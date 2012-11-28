#include "IfAST.hpp"
#include "../Utils.hpp"

IfAST::IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
               boost::weak_ptr<ASTNode> parent, int lineNo)
               : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void IfAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Bad conditional statement due " +
                             "to earlier error.");
    } else if (_expr->getType()->getTypeName() != "Boolean") {
        Utils::printSemErr(_lineNo, (string) "Conditional statement must be " +
                             "a Boolean, not a " +
                             _expr->getType()->getTypeName());
    }
}

void IfAST::print() {
    cout << "Conditional statement" << endl;
}
