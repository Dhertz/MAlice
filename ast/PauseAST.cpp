#include "PauseAST.hpp"
#include "../Utils.hpp"

PauseAST::PauseAST(boost::shared_ptr<SymbolTable> st,
                 boost::shared_ptr<ExprAST> expr,
                 boost::weak_ptr<ASTNode> parent, int lineNo)
                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void PauseAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot sleep due to an earlier " +
                             "error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Sleep duration must be a Number, not a " +
                             _expr->getType()->getTypeName());
    }
}
    
void PauseAST::print() {
    cout << "Pause statement" << endl;
}

string PauseAST::getNodeName() {
    return "Pause";
}

void PauseAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
     v->visitPause(_expr, _st, func);
}
