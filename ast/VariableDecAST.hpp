#ifndef VARIABLE_DEC_AST_H
#define VARIABLE_DEC_AST_H

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Variable.hpp"

class VariableDecAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	string _typeName;
	string _varName;
	boost::shared_ptr<Variable> _varObj;

public:
	VariableDecAST(boost::shared_ptr<SymbolTable> st, string typeName, string varName);
	void check();
};

#endif
