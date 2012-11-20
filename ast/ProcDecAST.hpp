#ifndef PROC_DEC_AST
#define PROC_DEC_AST

#include "ASTNode.hpp"
#include "HeaderParamsAST.hpp"
#include "../idents/Proc.hpp"
#include <vector>

class ProcDecAST : public ASTNode {
	SymbolTable* _st;
	string _name;
	string _returnType;
	HeaderParamsAST* _params;
	Proc* _procObj;
public:
	ProcDecAST(SymbolTable* st, string name, HeaderParamsAST* params);
	void check();
};

#endif