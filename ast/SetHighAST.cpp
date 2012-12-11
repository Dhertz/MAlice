#include "SetHighAST.hpp"
#include "../Utils.hpp"

SetHighAST::SetHighAST(boost::shared_ptr<SymbolTable> st,
                 boost::shared_ptr<ExprAST> expr,
                 boost::weak_ptr<ASTNode> parent, int lineNo)
                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void SetHighAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot set pin to high due to " +
                             "an earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Pin number must be a Number, not a " +
                             _expr->getType()->getTypeName());
    }
}
    
void SetHighAST::print() {
    cout << "SetHigh statement" << endl;
}

string SetHighAST::getNodeName() {
    return "SetHigh";
}

void SetHighAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
     v->visitInc(_expr, _st, func);
}
