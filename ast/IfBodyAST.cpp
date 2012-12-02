#include "IfBodyAST.hpp"

IfBodyAST::IfBodyAST(boost::shared_ptr<SymbolTable> st,
          			   boost::weak_ptr<ASTNode> parent, int lineNo)
					   : ASTNode(st, parent, lineNo) {}

void IfBodyAST::print() {
	cout << "If statement body" << endl;
}

string IfBodyAST::getNodeName() {
	return "IfBody";
}

void IfBodyAST::check() {}

void IfBodyAST::accept(boost::shared_ptr<ASTVisitor> v) {
	vector< boost::shared_ptr<ASTNode> >::iterator i;

	for (i = _children.begin(); i != _children.end(); ++i) {
		(*i)->accept(v);
	}
}
