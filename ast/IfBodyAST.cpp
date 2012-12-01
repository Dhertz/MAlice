#include "IfBodyAST.hpp"

IfBodyAST::IfBodyAST(boost::shared_ptr<SymbolTable> st,
          			   boost::weak_ptr<ASTNode> parent, int lineNo)
					   : ASTNode(st, parent, lineNo) {}

void IfBodyAST::print() {
	cout << "If statement body" << endl;
}
