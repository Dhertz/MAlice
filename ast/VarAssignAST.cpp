#include "VarAssignAST.hpp"
#include "../Utils.hpp"

VarAssignAST::VarAssignAST(boost::shared_ptr<SymbolTable> st, string varName,
                             boost::shared_ptr<ExprAST> expr,
                             boost::weak_ptr<ASTNode> parent, int lineNo)
                             : ASTNode(st, parent, lineNo) {
    _st = st;
    _varName = varName;
    _expr = expr;
    _lineNo = lineNo;
    check();
}

void VarAssignAST::check() {
     boost::shared_ptr<Identifier> var =
       _st->lookupCurrLevelAndEnclosingLevels(_varName);

    if (!var) {
        Utils::printSemErr(_lineNo, _varName + " is not in scope!");
    } else if (var->getBaseName() != "Variable") {
        Utils::printSemErr(_lineNo, _varName + " is not assignable.");
    } else {
        boost::shared_ptr<Variable> varCasted =
          boost::shared_polymorphic_downcast<Variable>(var);

        if(!_expr || !_expr->getType()) {
            Utils::printSemErr(_lineNo, "Cannot assign " + _varName + " to a " +
                "bad expression due to an earlier error.");
        } else if (!varCasted->getTypeName()) {
            Utils::printSemErr(_lineNo, "Cannot assign to " + _varName +
                " as it has a bad type due to an earlier error.");
        } else {
            string lhsType = varCasted->getTypeName()->getTypeName();
            string rhsType =_expr->getType()->getTypeName();

            if (lhsType != rhsType) {
                Utils::printSemErr(_lineNo, "Cannot assign to " + _varName +
                " as the lhs (" + lhsType + ") and rhs (" + rhsType + ") are " +
                "not type compatible.");
            }
        }
    }
}

void VarAssignAST::print() {
    cout << "Var assign node" << endl;
}
