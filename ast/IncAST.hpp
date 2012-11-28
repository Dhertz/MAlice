#ifndef INC_AST
#define INC_AST

#include "ExprAST.hpp"

class IncAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    IncAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	     boost::weak_ptr<ASTNode> parent, int lineNo);
};

#endif
