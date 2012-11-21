#include "ExprAST.hpp"
#include "../idents/Variable.hpp"
#include "../idents/Letter.hpp"
#include "../idents/Sentence.hpp"
#include "../idents/Number.hpp"
#include "../idents/Array.hpp"
#include "../idents/Boolean.hpp"

// Stolen from Owen. If these stay, might be best to move them to a Utils class or something
void printMe(pANTLR3_BASE_TREE tree, int level) {
        for (int i = 0; i < level; ++i) {
                cout << "--";
        }

        cout << " " << tree->getText(tree)->chars << endl;

        int childCount = tree->getChildCount(tree);
        for (int i = 0; i < childCount; ++i) {
                printMe((pANTLR3_BASE_TREE) tree->getChild(tree, i), level + 1);
        }
}

void printTreeMe(pANTLR3_BASE_TREE ast) {
        printMe(ast, 0);
}

pANTLR3_BASE_TREE ExprAST::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string ExprAST::createStringFromTree(pANTLR3_BASE_TREE tree) {
	string res((const char *) tree->getText(tree)->to8(tree->getText(tree))->chars, 
			   tree->getText(tree)->len);
	return res;
}

ExprAST::ExprAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_boolArgBoolRet.insert("!");

	_intArgIntRet.insert("~");
	_intArgIntRet.insert("+");
	_intArgIntRet.insert("-");

	_boolArgsBoolRet.insert("||");
	_boolArgsBoolRet.insert("&&");

	// Mixed: int or char (LHS and RHS must match)
	_mixedArgsMixedRet.insert("|");
	_mixedArgsMixedRet.insert("^");
	_mixedArgsMixedRet.insert("&");
	_mixedArgsMixedRet.insert("&");
	_mixedArgsMixedRet.insert("&");
	_mixedArgsMixedRet.insert("+");
	_mixedArgsMixedRet.insert("-");
	_mixedArgsMixedRet.insert("*");
	_mixedArgsMixedRet.insert("/");
	_mixedArgsMixedRet.insert("%");

	_mixedArgsBoolRet.insert("==");
	_mixedArgsBoolRet.insert("!=");
	_mixedArgsBoolRet.insert(">");
	_mixedArgsBoolRet.insert("<");
	_mixedArgsBoolRet.insert(">=");
	_mixedArgsBoolRet.insert("<=");

	_type = boost::shared_ptr<Type>();
	_tree = tree;

	check();
}

void ExprAST::check() {
	int children = _tree->getChildCount(_tree);
	assert (children > 0);

	pANTLR3_BASE_TREE root = childByNum(_tree, 0);
	string tok = createStringFromTree(root);

	if (tok == "FUNC") {
		// Inline function call, evaluates to function's return type

		string funcName = createStringFromTree(childByNum(root, 0));
		pANTLR3_BASE_TREE cplTree = childByNum(root, 1);

		// Check that function call and parameters type-check
		// i.e. function name in scope, and parameters exprs match expected type
		boost::shared_ptr<CallParamsAST> callParamsNode = boost::shared_ptr<CallParamsAST>(new CallParamsAST(_st, cplTree, _parent));
		FuncAST(_st, funcName, callParamsNode, _parent);

		// This cast should be safe after FuncAST has done its work
		boost::shared_ptr<Identifier> funcIdent = _st->lookupCurrLevelAndEnclosingLevels(funcName);
		boost::shared_ptr<Function> func = boost::shared_polymorphic_downcast<Function>(funcIdent);

		_type = func->getTypeName();
	} else if (tok == "VAR") {
		// Variable reference, evaluates to variable's type

		string varName = createStringFromTree(childByNum(root, 0));

		boost::shared_ptr<Identifier> varIdent = _st->lookupCurrLevelAndEnclosingLevels(varName);
		if (!varIdent) {
			cerr << varName << " is not in scope" << endl;
		} else if (varIdent->getBaseName() != "Variable") {
			cerr << varName << " is not a variable. It is a " << varIdent->getBaseName() << "." << endl;
		} else {
			boost::shared_ptr<Variable> var = boost::shared_polymorphic_downcast<Variable>(varIdent);
			_type = var->getTypeName();
		}
	} else if (tok == "ARRMEMBER") {
		// Array member reference, evaluates to array's element type

		string arrName = createStringFromTree(childByNum(root, 0));

		boost::shared_ptr<Identifier> arrIdent = _st->lookupCurrLevelAndEnclosingLevels(arrName);
		if (!arrIdent) {
			cerr << arrName << " is not in scope" << endl;
		} else if (arrIdent->getBaseName() != "Type") {
			cerr << arrName << " is not an array" << endl;
		} else {
			boost::shared_ptr<Type> arrType = boost::shared_polymorphic_downcast<Type>(arrIdent);
			if (arrType->getTypeName() != "Array") {
				cerr << arrName << " is not an array" << endl;
			} else {
				boost::shared_ptr<Array> arr = boost::shared_polymorphic_downcast<Array>(arrIdent);

				pANTLR3_BASE_TREE index = childByNum(root, 1);
				ExprAST indexCheck(_st, index, _parent);

				// Need to cast to Type before second getTypeName()?
				if (indexCheck.getTypeName()->getTypeName() != "Number") {
					cerr << "Array index must evaluate to a Number." << endl;
				} else {
					_type = arr->getElemType();
				}
			}
		}
		
	} else if (tok[0] == '\'') {
		// Char of form 'x', evaluates to a Letter
		boost::shared_ptr<Type> letter = boost::shared_ptr<Type>(new Letter);
		_type = letter;
	} else if (tok[0] == '"') {
		// String of form "foo", evaluates to a Sentence
		boost::shared_ptr<Type> sentence = boost::shared_ptr<Type>(new Sentence);
		_type = sentence;
	} else {
		// Recursive case, will resolve to a boolean or number

		boost::shared_ptr<Type> retType = recurseTree(root, "*");
		assert(retType);
		_type = retType;
	}
}

boost::shared_ptr<Type> ExprAST::recurseTree(pANTLR3_BASE_TREE tree, string expectedType) {
	int children = tree->getChildCount(tree);
	assert (0 <= children && children < 3);
	if (children == 0) {
		// Base case

		// TODO: remove need for constructor arguments here
		boost::shared_ptr<Type> number = boost::shared_ptr<Type>(new Number(0, 1));
		return number;
	} else if (children == 1) {
		// Unary operator
		string op = createStringFromTree(tree);
		pANTLR3_BASE_TREE arg = childByNum(tree, 0);

		string expEvalType;
		boost::shared_ptr<Type> evaluatedType;
		
		if (_intArgIntRet.find(op) != _intArgIntRet.end()) {
			expEvalType = "Number";
		} else if (_boolArgBoolRet.find(op) != _boolArgBoolRet.end()) {
			expEvalType = "Boolean";
		} else {
			ExprAST checkExp(_st, tree, _parent);
			evaluatedType = checkExp.getTypeName();

			if (expectedType != "*" && expectedType != evaluatedType->getTypeName()) {
				cerr << "Expected argument expression to " << op << " to evaluate to a " << expectedType << ", but got a " << evaluatedType->getTypeName() << endl;
				return boost::shared_ptr<Type>();
			}
			
			return evaluatedType;
		}

		evaluatedType = recurseTree(arg, expEvalType);

		if (expectedType != "*" && expectedType != evaluatedType->getTypeName()) {
			cerr << "Argument to " << op << " should be a " << expectedType << ", but got " << evaluatedType->getTypeName() << endl;
			return boost::shared_ptr<Type>();
		} else {
			return evaluatedType;
		}
	} else if (children == 2) {
		// Binary operator
		string op = createStringFromTree(tree);
		pANTLR3_BASE_TREE lhs = childByNum(tree, 0);
		pANTLR3_BASE_TREE rhs = childByNum(tree, 1);

		string lhsTok = createStringFromTree(lhs);
		string rhsTok = createStringFromTree(rhs);

		boost::shared_ptr<Type> lhsType, rhsType;

		if (_mixedArgsMixedRet.find(op) != _mixedArgsMixedRet.end()) {
			// args must be of same type
			// return type is type of either arg
			lhsType = recurseTree(lhs, "*");
			rhsType = recurseTree(rhs, "*");
			if (lhsType->getTypeName() != rhsType->getTypeName()) {
				cerr << "Operand type mismatch in " << lhsTok << " " << op << " " << rhsTok
					<< " (" << lhsType->getTypeName() << " isn't the same as " << rhsType->getTypeName() << ")" << endl;
				return boost::shared_ptr<Type>();
			} else if (expectedType != "*" && lhsType->getTypeName() != expectedType) {
				cerr << "In this scenario, " << op << " will return a " << lhsType->getTypeName() << ", not a " << expectedType << endl;
				return boost::shared_ptr<Type>();
			}
			return lhsType;
		} else if (_mixedArgsBoolRet.find(op) != _mixedArgsBoolRet.end()) {
			// args must be of same type

			if (expectedType != "*" && expectedType != "Boolean") {
				// No point even evaluating lhs or rhs because my return type is wrong
				cerr << op << " returns a Boolean, not a " << expectedType << endl;
				return boost::shared_ptr<Type>();
			} else {
				lhsType = recurseTree(lhs, "*");
				rhsType = recurseTree(rhs, "*");
				if (lhsType->getTypeName() != rhsType->getTypeName()) {
					cerr << "Operand type mismatch in " << lhsTok << " " << op << " " << rhsTok
						<< " (" << lhsType->getTypeName() << " isn't the same as " << rhsType->getTypeName() << ")" << endl;
					return boost::shared_ptr<Type>();
				}

				// return new boolean
				boost::shared_ptr<Type> evaluatedType = boost::shared_ptr<Type>(new Boolean);
				return evaluatedType;
			}
		} else if (_boolArgsBoolRet.find(op) != _boolArgsBoolRet.end()) {
			// args must be boolean

			if (expectedType != "*" && expectedType != "Boolean") {
				// No point even evaluating lhs or rhs because my return type is wrong
				cerr << op << " returns a Boolean, not a " << expectedType << endl;
				return boost::shared_ptr<Type>();
			} else {
				lhsType = recurseTree(lhs, "Boolean");
				rhsType = recurseTree(rhs, "Boolean");

				// return new boolean
				boost::shared_ptr<Type> evaluatedType = boost::shared_ptr<Type>(new Boolean);
				return evaluatedType;
			}
		} else {
			ExprAST checkExp(_st, tree, _parent);
			boost::shared_ptr<Type> evaluatedType = checkExp.getTypeName();

			if (expectedType != "*" && expectedType != evaluatedType->getTypeName()) {
				cerr << "Expected argument expression to " << op << " to evaluate to a " << expectedType << ", but got a " << evaluatedType->getTypeName() << endl;
				return boost::shared_ptr<Type>();
			}
			
			return evaluatedType;
		}
	}
}

boost::shared_ptr<Type> ExprAST::getTypeName() {
	return _type;
}
