#ifndef VARIABLE_DEC_AST_H
#define VARIABLE_DEC_AST_H

#include "ExprAST.hpp"
#include "../idents/Variable.hpp"

class VariableDecAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _typeName;
    string _varName;
    boost::shared_ptr<Variable> _varObj;
    int _lineNo;
    void check();
public:
    VariableDecAST(boost::shared_ptr<SymbolTable> st, string typeName, string varName, boost::shared_ptr<ASTNode> parent, int lineNo);
};

#endif
