#include "DecAST.hpp"
#include "../Utils.hpp"

DecAST::DecAST(boost::shared_ptr<SymbolTable> st,
                 boost::shared_ptr<ExprAST> expr,
                 boost::weak_ptr<ASTNode> parent, int lineNo)
                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void DecAST::check() {
    if (!_expr || !_expr->getType()) {
		Utils::printSemErr(_lineNo, (string) "Invalid decrement argument " +
                           "due to earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, (string) "Decrement argument must be a " +
                           "Number, not a " +
                           _expr->getType()->getTypeName() + ".");
    }
}

void DecAST::print() {
    cout << "Decrement statement" << endl;
}
