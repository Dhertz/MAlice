#ifndef MAKE_OUT_AST
#define MAKE_OUT_AST

#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class MakeOutAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    MakeOutAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	     boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
