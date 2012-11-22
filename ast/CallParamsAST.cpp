#include "CallParamsAST.hpp"

pANTLR3_BASE_TREE CallParamsAST::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

CallParamsAST::CallParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_tree = tree;
	_lineNo = lineNo;
	for (int i = 0; i < _tree->getChildCount(_tree); ++i) {
		// This line number is probably correct, might be worht checking though - Owen
		boost::shared_ptr<ExprAST> e(new ExprAST(_st, childByNum(_tree, i), parent, _lineNo));
		_types.push_back(e->getTypeName());
	}
}

vector< boost::shared_ptr<Type> > CallParamsAST::getTypes() {
	return _types;
}
