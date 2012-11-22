#include "CallParamsAST.hpp"
#include "ExprAST.hpp"

CallParamsAST::CallParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_tree = tree;
	_parent = parent;
	_lineNo = lineNo;
	check();
}

vector< boost::shared_ptr<Type> > CallParamsAST::getTypes() {
	return _types;
}

void CallParamsAST::check() {
	for (int i = 0; i < _tree->getChildCount(_tree); ++i) {
		// This line number is probably correct, might be worht checking though - Owen
		boost::shared_ptr<ExprAST> e(new ExprAST(_st, TreeUtils::childByNum(_tree, i), _parent, _lineNo));
		_types.push_back(e->getTypeName());
	}
}
