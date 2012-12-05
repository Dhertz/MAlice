#ifndef CHOICE_AST
#define CHOICE_AST

#include "../ASTVisitor.hpp"
#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "IfBodyAST.hpp"

class ChoiceAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    boost::shared_ptr<IfBodyAST> _ifs;
    boost::shared_ptr<IfBodyAST> _elses;
    int _lineNo;
    void check();
public:
    ChoiceAST(boost::shared_ptr<SymbolTable> st,
                boost::shared_ptr<ExprAST> expr,
                boost::shared_ptr<IfBodyAST> ifs,
                boost::shared_ptr<IfBodyAST> elses,
                boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
	void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
