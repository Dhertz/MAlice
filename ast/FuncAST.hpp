#ifndef FUNC_AST
#define FUNC_AST

#include "CallParamsAST.hpp"
#include "../idents/Callable.hpp"

class FuncAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _name;
    boost::shared_ptr<CallParamsAST> _params;
    int _lineNo;
    void parametersTypeCheck(boost::shared_ptr<Callable> function);
    void check();
public:
    FuncAST(boost::shared_ptr<SymbolTable> st, string name,
    	      boost::shared_ptr<CallParamsAST> params,
    	      boost::shared_ptr<ASTNode> parent, int lineNo);
};

#endif
