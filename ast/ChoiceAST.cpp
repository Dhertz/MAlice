#include "ChoiceAST.hpp"
#include "../Utils.hpp"

ChoiceAST::ChoiceAST(boost::shared_ptr<SymbolTable> st,
                       boost::shared_ptr<ExprAST> expr,
                       boost::shared_ptr<IfBodyAST> ifs,
                       boost::shared_ptr<IfBodyAST> elses,
                       boost::weak_ptr<ASTNode> parent, int lineNo)
                       : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _ifs = ifs;
    _elses = elses;
    _lineNo = lineNo;
    check();
}

void ChoiceAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Bad conditional statement due " +
                             "to an earlier error.");
    } else if (_expr->getType()->getTypeName() != "Boolean") {
        Utils::printSemErr(_lineNo, (string) "Conditional statement must be " +
                             "a Boolean, not a " +
                             _expr->getType()->getTypeName() + ".");
    }
}

void ChoiceAST::print() {
    cout << "Choice conditional" << endl;
}

string ChoiceAST::getNodeName() {
    return "Choice";
}

void ChoiceAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
	v->visitChoice(_expr, _ifs, _elses, _st, func);
}
