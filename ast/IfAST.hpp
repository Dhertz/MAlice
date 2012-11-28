#ifndef IF_AST
#define IF_AST

#include "ExprAST.hpp"

class IfAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	    boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
};

#endif
