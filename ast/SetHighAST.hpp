#ifndef SET_HIGH_AST
#define SET_HIGH_AST

#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class SetHighAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    SetHighAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	     boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
