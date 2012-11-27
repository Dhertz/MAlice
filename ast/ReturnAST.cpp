#include "ReturnAST.hpp"
#include "FuncDecAST.hpp"

ReturnAST::ReturnAST(boost::shared_ptr<SymbolTable> st,
                       boost::shared_ptr<ExprAST> expr,
                       boost::shared_ptr<ASTNode> parent, int lineNo)
                       : ASTNode(st, parent, lineNo) {
    _st = st;
    _expr = expr;
    _parent = parent;
    _lineNo = lineNo;
    check();
}

void ReturnAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot return bad expression " +
                             "due to an earlier error.");
    } else {
        checkFunctionType(_parent);
    }
}

void ReturnAST::checkFunctionType(boost::shared_ptr<ASTNode> parent) {
    if (!parent) {
        // Shouldn't happen, as this case would be caught by a parse error
        return;
    }

    if (parent->getNodeName() == "FuncDec") {
            boost::shared_ptr<FuncDecAST> funcDec =
              boost::shared_polymorphic_downcast<FuncDecAST>(parent);

            string funcName = funcDec->getFuncName();

            boost::shared_ptr<Identifier> funcId = _st->lookupCurrLevelAndEnclosingLevels(funcName);

            if (!funcId) {
                Utils::printSemErr(_lineNo, funcName + "is not in scope.");
            } else if (funcId->getBaseName() != "Callable") {
                Utils::printSemErr(_lineNo, funcName + "is not a function.");
            } else {
                boost::shared_ptr<Callable> funcCallable = boost::shared_polymorphic_downcast<Callable>(funcId);
                if (funcCallable->getCallableFuncOrProc() != "Function") {
                    Utils::printSemErr(_lineNo, funcName + "is not a function.");
                } else {
                    boost::shared_ptr<Function> func = boost::shared_polymorphic_downcast<Function>(funcCallable);
                    if (!func->getType()) {
                        Utils::printSemErr(_lineNo, "Function return type is bad due to an earlier error.");
                    } else if (!_expr || !_expr->getType()) {
                        Utils::printSemErr(_lineNo, "Return expression is bad due to an earlier error.");
                    } else {
                        string funcReturnType = func->getType()->getTypeName();
                        string exprType = _expr->getType()->getTypeName();

                        if (funcReturnType != exprType) {
                            Utils::printSemErr(_lineNo, "Return type for " + funcName + " (" + funcReturnType + ") does not match type of return expression (" + exprType + ").");
                        }
                    }
                }
            }
    } else {
        checkFunctionType(parent->getParent());
    }
}
