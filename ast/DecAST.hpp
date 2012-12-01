#ifndef DEC_AST
#define DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class DecAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    DecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
             boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
