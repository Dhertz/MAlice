#include "ExprAST.hpp"
#include "../idents/Variable.hpp"
#include "../idents/Letter.hpp"
#include "../idents/Sentence.hpp"
#include "../idents/Number.hpp"
#include "../idents/Array.hpp"

// Stolen from Owen. If these stay, might be best to move them to a Utils class or something
pANTLR3_BASE_TREE ExprAST::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string ExprAST::createStringFromTree(pANTLR3_BASE_TREE tree) {
	string res((const char *) tree->getText(tree)->to8(tree->getText(tree))->chars, 
			   tree->getText(tree)->len);
	return res;
}

ExprAST::ExprAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_unaryOps.insert("!");
	_unaryOps.insert("~");
	_unaryOps.insert("+");
	_unaryOps.insert("-");

	_binaryOps.insert("||");
	_binaryOps.insert("&&");
	_binaryOps.insert("|");
	_binaryOps.insert("^");
	_binaryOps.insert("&");
	_binaryOps.insert("==");
	_binaryOps.insert("!=");
	_binaryOps.insert(">");
	_binaryOps.insert("<");
	_binaryOps.insert(">=");
	_binaryOps.insert("<=");
	_binaryOps.insert("+");
	_binaryOps.insert("-");
	_binaryOps.insert("*");
	_binaryOps.insert("/");
	_binaryOps.insert("%");

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
		boost::shared_ptr<Type> sentence = boost::shared_ptr<Sentence>(new Sentence);
		_type = sentence;
	} else {
		// Recursive case, will resolve to an (internal) boolean or a number
		// cout << "TODO: recursive case" << endl;

		// TODO: remove need for the constructor here
		boost::shared_ptr<Type> number = boost::shared_ptr<Number>(new Number(0, 1));
		_type = number;
	}
}

boost::shared_ptr<Type> ExprAST::getTypeName() {
	return _type;
}
