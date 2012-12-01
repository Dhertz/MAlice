#include "StdinAST.hpp"
#include "../Utils.hpp"

StdinAST::StdinAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::weak_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void StdinAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot read in to bad " +
                             "expression due to earlier error.");
    } else if (!_expr->isAssignable()) {
        Utils::printSemErr(_lineNo, (string) "Can only read in to variables " +
                             " or array elements.");
    }
}

void StdinAST::print() {
    cout << "Read in node" << endl;
}

void StdinAST::accept(boost::shared_ptr<ASTVisitor> v) {
    v->visitSTDIN(_expr);
}