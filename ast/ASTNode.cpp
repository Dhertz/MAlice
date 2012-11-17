#include "ASTNode.hpp"

ASTNode::ASTNode(SymbolTable* st) {
	_st = st;
}

vector<ASTNode*> ASTNode::getChildren() {
	return _children;
}

void ASTNode::addChild(ASTNode* child, int childNum) {
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
