#ifndef FUNC_PROC_CALL_AST
#define FUNC_PROC_CALLAST

#include "CallParamsAST.hpp"
#include "../idents/Callable.hpp"
#include "../ASTVisitor.hpp"

class FuncProcCallAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _name;
    boost::shared_ptr<CallParamsAST> _params;
    int _lineNo;
    void parametersTypeCheck(boost::shared_ptr<Callable> function);
    void check();
public:
    FuncProcCallAST(boost::shared_ptr<SymbolTable> st, string name,
    	      boost::shared_ptr<CallParamsAST> params,
    	      boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
