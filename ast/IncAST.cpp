#include "IncAST.hpp"
#include "../Utils.hpp"

IncAST::IncAST(boost::shared_ptr<SymbolTable> st,
                 boost::shared_ptr<ExprAST> expr,
                 boost::weak_ptr<ASTNode> parent, int lineNo)
                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void IncAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot increment bad " +
                             "expression due to earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Can only increment a Number, not a " +
                             _expr->getType()->getTypeName());
    }
}
    
void IncAST::print() {
    cout << "Increment statement" << endl;
}

void IncAST::accept(boost::shared_ptr<ASTVisitor> v) {
    
}
