#include "PrintAST.hpp"

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::weak_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void PrintAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot print bad expression " +
                             "due to earlier error.");
    } else if (_expr->getType()->getTypeName() == "Array") {
        Utils::printSemErr(_lineNo, (string) "Cannot print an array.");
    }
}

void PrintAST::print() {
    cout << "Print node" << endl;
}

string PrintAST::getNodeName() {
    return "Print";
}

void PrintAST::accept(boost::shared_ptr<ASTVisitor> v) {
    v->visitPrint(_expr, _st);
}