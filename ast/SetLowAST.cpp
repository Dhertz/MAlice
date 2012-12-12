#include "SetLowAST.hpp"
#include "../Utils.hpp"

SetLowAST::SetLowAST(boost::shared_ptr<SymbolTable> st,
                 boost::shared_ptr<ExprAST> expr,
                 boost::weak_ptr<ASTNode> parent, int lineNo)
                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void SetLowAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot set pin to low due to " +
                             "an earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Pin number must be a Number, not a " +
                             _expr->getType()->getTypeName());
    }
}
    
void SetLowAST::print() {
    cout << "SetLow statement" << endl;
}

string SetLowAST::getNodeName() {
    return "SetLow";
}

void SetLowAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
     v->visitSetLow(_expr, _st, func);
}
