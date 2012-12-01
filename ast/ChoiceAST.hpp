#ifndef CHOICE_AST
#define CHOICE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ChoiceAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    ChoiceAST(boost::shared_ptr<SymbolTable> st,
                boost::shared_ptr<ExprAST> expr,
                boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
	void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
