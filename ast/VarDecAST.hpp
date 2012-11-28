#ifndef VAR_DEC_AST_H
#define VAR_DEC_AST_H

#include "ExprAST.hpp"
#include "../idents/Variable.hpp"

class VarDecAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _typeName;
    string _varName;
    int _lineNo;
    void check();
public:
    VarDecAST(boost::shared_ptr<SymbolTable> st, string typeName,
    	             string varName, boost::weak_ptr<ASTNode> parent,
    	             int lineNo);
    void print();
};

#endif
