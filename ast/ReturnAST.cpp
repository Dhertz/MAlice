#include "ReturnAST.hpp"
#include "FuncDecAST.hpp"
#include "ProcDecAST.hpp"

ReturnAST::ReturnAST(boost::shared_ptr<SymbolTable> st,
                       boost::shared_ptr<ExprAST> expr,
                       boost::weak_ptr<ASTNode> parent, int lineNo)
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

void ReturnAST::checkFunctionType(boost::weak_ptr<ASTNode> parent) {
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        if (!p) {
            // Shouldn't happen, as this case would be caught by a parse error
            return;
        }
        if (p->getNodeName() == "FuncDec") {
            boost::shared_ptr<FuncDecAST> funcDec =
              boost::shared_polymorphic_downcast<FuncDecAST>(p);

            string funcName = funcDec->getFuncName();

            boost::shared_ptr<Identifier> funcId = 
              _st->lookupCurrLevelAndEnclosingLevels(funcName);

            if (!funcId) {
                Utils::printSemErr(_lineNo, funcName + "is not in scope.");
            } else if (funcId->getBaseName() != "Callable") {
                Utils::printSemErr(_lineNo, funcName + "is not a function.");
            } else {
                boost::shared_ptr<Callable> funcCallable = 
                  boost::shared_polymorphic_downcast<Callable>(funcId);
                if (funcCallable->getCallableFuncOrProc() != "Function") {
                    Utils::printSemErr(_lineNo, funcName +
                                         "is not a function.");
                } else {
                    boost::shared_ptr<Function> func = 
                     boost::shared_polymorphic_downcast<Function>(funcCallable);
                    if (!func->getType()) {
                        Utils::printSemErr(_lineNo, (string) "Function return" + 
                                             " type is bad due to an earlier " + 
                                             "error.");
                    } else if (!_expr || !_expr->getType()) {
                        Utils::printSemErr(_lineNo, (string) "Return " + 
                                            "expression is bad due to an " + 
                                            "earlier error.");
                    } else {
                        string funcReturnType = func->getType()->getTypeName();
                        string exprType = _expr->getType()->getTypeName();

                        if (funcReturnType != exprType) {
                            Utils::printSemErr(_lineNo, "Return type for " + 
                                                funcName + " (" + 
                                                funcReturnType + ") does not " + 
                                                "match type of return " + 
                                                "expression (" + exprType + 
                                                ").");
                        }
                    }
                }
            }
        } else if (p->getNodeName() == "ProcDec") {
            boost::shared_ptr<ProcDecAST> procDec =
              boost::shared_polymorphic_downcast<ProcDecAST>(p);
            Utils::printSemErr(_lineNo, "Return statement found in procedure " 
                                 + procDec->getProcName());
        } else {
            checkFunctionType(p->getParent());
        }
    }
}

void ReturnAST::print() {
    cout << "Function return" << endl;
}

void ReturnAST::accept(boost::shared_ptr<ASTVisitor> v) {
	v->visitReturn(_expr);
}
