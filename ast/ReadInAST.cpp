#include "ReadInAST.hpp"
#include "../Utils.hpp"

ReadInAST::ReadInAST(boost::shared_ptr<SymbolTable> st, 
                       boost::shared_ptr<ExprAST> expr, 
                       boost::weak_ptr<ASTNode> parent, int lineNo) 
                       : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void ReadInAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot read in to bad " +
                             "expression due to earlier error.");
    } else if (!_expr->isAssignable()) {
        Utils::printSemErr(_lineNo, (string) "Can only read in to variables " +
                             " or array elements.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, (string) "Can only read GPIO input into " +
                             "a Number, not a " +
                             _expr->getType()->getTypeName());
    }
}

void ReadInAST::print() {
    cout << "Read GPIO input node" << endl;
}

string ReadInAST::getNodeName() {
    return "ReadIn";
}

void ReadInAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
     v->visitReadIn(_expr, _st, func);
}
