#include "MakeOutAST.hpp"
#include "../Utils.hpp"

MakeOutAST::MakeOutAST(boost::shared_ptr<SymbolTable> st,
                 boost::shared_ptr<ExprAST> expr,
                 boost::weak_ptr<ASTNode> parent, int lineNo)
                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void MakeOutAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot set pin type due to " +
                             "an earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Pin number must be a Number, not a " +
                             _expr->getType()->getTypeName());
    }
}
    
void MakeOutAST::print() {
    cout << "MakeOut statement" << endl;
}

string MakeOutAST::getNodeName() {
    return "MakeOut";
}

void MakeOutAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
     v->visitMakeOut(_expr, _st, func);
}
