#include "WhileAST.hpp"
#include "../Utils.hpp"

WhileAST::WhileAST(boost::shared_ptr<SymbolTable> st,
	                 boost::shared_ptr<ExprAST> expr,
	                 boost::weak_ptr<ASTNode> parent, int lineNo)
                     : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void WhileAST::check() {
    if (!_expr || !_expr->getType()) {
    	Utils::printSemErr(_lineNo, (string) "Bad conditional statement due " +
    		                 "to an earlier error.");
    } else if (_expr->getType()->getTypeName() != "Boolean") {
    	Utils::printSemErr(_lineNo, (string) "Eventually conditional " +
    		                 "statement must be a Boolean, not a " +
    		                 _expr->getType()->getBaseName() + ".");
    }
}

void WhileAST::print() {
    cout << "While Node" << endl;
}

void WhileAST::accept(boost::shared_ptr<ASTVisitor> v) {
    v->visitWHILE(_expr);
}
