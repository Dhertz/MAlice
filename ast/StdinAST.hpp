#ifndef STDIN_AST
#define STDIN_AST

#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class StdinAST : public ASTNode {
    boost::shared_ptr<SymbolTable> st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    StdinAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	       boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
