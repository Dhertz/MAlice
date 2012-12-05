#include "ASTNode.hpp"
#include <iostream>

ASTNode::ASTNode(boost::shared_ptr<SymbolTable> st,
                   boost::weak_ptr<ASTNode> parent, int lineNo) {
    _st = st;
    _parent = parent;
    _lineNo = lineNo;
}

vector< boost::shared_ptr<ASTNode> > ASTNode::getChildren() {
    return _children;
}

void ASTNode::addChild(boost::shared_ptr<ASTNode> child) {
	_children.push_back(child);
}

boost::weak_ptr<ASTNode> ASTNode::getParent() {
    return _parent;
}

void ASTNode::accept(boost::shared_ptr<ASTVisitor> v) {}

void ASTNode::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {}
