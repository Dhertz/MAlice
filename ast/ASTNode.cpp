#include "ASTNode.hpp"

ASTNode::ASTNode(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ASTNode> parent) {
	_st = st;
    _parent = parent;
}

vector< boost::shared_ptr<ASTNode> > ASTNode::getChildren() {
	return _children;
}

void ASTNode::addChild(boost::shared_ptr<ASTNode> child, int childNum) {
	if (_children.begin() + childNum >= _children.end()) {
		_children.push_back(child);
	} else {
		_children.insert(_children.begin() + childNum, child);
	}
}

void ASTNode::check() {}

void ASTNode::print() {
	cout << "Node" << endl;
}

// I haven't overriden this everywhere, because I only need it for FuncDec at the moment - Owen
string ASTNode::getNodeName() {
	return "Node";
}

boost::shared_ptr<ASTNode> ASTNode::getParent() {
    return _parent;
}
