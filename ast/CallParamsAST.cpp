#include "CallParamsAST.hpp"

pANTLR3_BASE_TREE CallParamsAST::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

CallParamsAST::CallParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree) : ASTNode(st) {
	_st = st;
	_tree = tree;
	for (int i = 0; i < _tree->getChildCount(_tree); ++i) {
		ExprAST e(_st, childByNum(_tree, i));
		_types.push_back(e.getType());
	}
}

vector<Type*> CallParamsAST::getTypes() {
	return _types;
}
