#include "ProgAST.hpp"

ProgAST::ProgAST(boost::shared_ptr<SymbolTable> st,
          		   boost::weak_ptr<ASTNode> parent, int lineNo)
				   : ASTNode(st, parent, lineNo) {}

void ProgAST::print() {
	cout << "Root node" << endl;
}

string ProgAST::getNodeName() {
	return "Prog";
}

void ProgAST::check() {}

void ProgAST::accept(ASTVisitor v) {
	v.visitProg(_children, _st);
}
