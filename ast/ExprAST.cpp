#include "ExprAST.hpp"
#include "../idents/Variable.hpp"

// Stolen from Owen. If these stay, might be best to move them to a Utils class or something
pANTLR3_BASE_TREE ExprAST::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string ExprAST::createStringFromTree(pANTLR3_BASE_TREE tree) {
	string res((const char *) tree->getText(tree)->to8(tree->getText(tree))->chars, 
			   tree->getText(tree)->len);
	return res;
}

ExprAST::ExprAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree) : ASTNode(st) {
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
		// Inline function call
		string funcName = createStringFromTree(childByNum(root, 0));
		pANTLR3_BASE_TREE cplTree = childByNum(root, 1);

		// Check that function call and parameters type-check
		// i.e. function name in scope, and parameters exprs match expected type
		boost::shared_ptr<CallParamsAST> callParamsNode = boost::shared_ptr<CallParamsAST>(new CallParamsAST(_st, cplTree));
		FuncAST(_st, funcName, callParamsNode);

		// This cast should be safe after FuncAST has done its work
		boost::shared_ptr<Identifier> funcIdent = _st->lookupCurrLevelAndEnclosingLevels(funcName);
		boost::shared_ptr<Function> func = boost::shared_polymorphic_downcast<Function>(funcIdent);

		_type = func->getTypeName();
	} else if (tok == "VAR") {
		string varName = createStringFromTree(childByNum(root, 0));

		boost::shared_ptr<Identifier> varIdent = _st->lookupCurrLevelAndEnclosingLevels(varName);
		if (!varIdent) {
			cerr << varName << " is not in scope" << endl;
		} else if (varIdent->getBaseName() != "Variable") {
			cerr << varName << " is not a variable" << endl;
		} else {
			boost::shared_ptr<Variable> var = boost::shared_polymorphic_downcast<Variable>(varIdent);
			_type = var->getTypeName();
		}
	}
}

boost::shared_ptr<Type> ExprAST::getTypeName() {
	return _type;
}
