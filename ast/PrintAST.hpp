#ifndef PRINT_AST
#define PRINT_AST

#include "CallParamsAST.hpp"
#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class PrintAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    boost::weak_ptr<ASTNode> parent, int lineNo);
    boost::shared_ptr<Identifier> getTypeName();
    boost::shared_ptr<ExprAST> getExpr();
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};
#endif
