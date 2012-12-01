#ifndef WHILE_AST
#define WHILE_AST

#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class WhileAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    WhileAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	       boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
