#include "ExprAST.hpp"

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

	if (children == 2) {
		// Inline function call
		string funcName = createStringFromTree(childByNum(_tree, 0));
		pANTLR3_BASE_TREE cplTree = childByNum(_tree, 1);

		if (createStringFromTree(cplTree) == "CPL") {
			// Check that function call and parameters type-check
			// i.e. function name in scope, and parameters exprs match expected type
			boost::shared_ptr<CallParamsAST> callParamsNode = boost::shared_ptr<CallParamsAST>(new CallParamsAST(_st, cplTree));
			FuncAST(_st, funcName, callParamsNode);

			// This cast should be safe after FuncAST has done its work
			boost::shared_ptr<Identifier> funcIdent = _st->lookupCurrLevelAndEnclosingLevels(funcName);
			boost::shared_ptr<Function> func = boost::shared_polymorphic_downcast<Function>(funcIdent);

			_type = func->getType();
		} else {
			cerr << "Invalid ExprAST node." << endl;
		}
	} else if (children == 1) {
		// Expression of type boolean or number

		string op = createStringFromTree(childByNum(_tree, 0));
		cout << "Op: " << op << endl;

		/* cout << children << " children." << endl << endl;
		for (int i = 0; i < children; ++i) {
			pANTLR3_BASE_TREE child = childByNum(_tree, i);
			cout << "Child " << i << ": " << createStringFromTree(child) << endl;
		} */
	} else {
		cerr << "Invalid ExprAST node." << endl;
	}
}

boost::shared_ptr<Type> ExprAST::getType() {
	return _type;
}
