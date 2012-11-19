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

ExprAST::ExprAST(SymbolTable* st, pANTLR3_BASE_TREE tree) : ASTNode(st) {
	_tree = tree;

	check();
}

void ExprAST::check() {
	int children = _tree->getChildCount(_tree);
	// cout << children << " children." << endl << endl;
	for (int i = 0; i < children; ++i) {
		pANTLR3_BASE_TREE child = childByNum(_tree, i);
		// cout << "Child " << i << ": " << createStringFromTree(child) << endl;
	}
}

Type* ExprAST::getType() {
	return _type;
}
