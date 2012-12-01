#ifndef VAR_ASSIGN_AST_H
#define VAR_ASSIGN_AST_H

#include "ExprAST.hpp"
#include "../idents/Variable.hpp"
#include "../ASTVisitor.hpp"

class VarAssignAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _varName;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    VarAssignAST(boost::shared_ptr<SymbolTable> st, string varName,
    	           boost::shared_ptr<ExprAST> expr,
    	           boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
