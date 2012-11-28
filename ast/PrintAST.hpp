#ifndef PRINT_AST
#define PRINT_AST

#include "CallParamsAST.hpp"
#include "ExprAST.hpp"

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
};

#endif
