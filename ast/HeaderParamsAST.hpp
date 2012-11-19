#ifndef HEADER_PARAMS_AST
#define HEADER_PARAMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>
#include <vector>
#include "../idents/Param.hpp"

class HeaderParamsAST : public ASTNode {

public:
	SymbolTable* _st;
	pANTLR3_BASE_TREE _tree;
	vector<Param> _params;

	HeaderParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	string createStringFromTree(pANTLR3_BASE_TREE tree);
	bool duplicate(int upto, string name);
	void check();
	vector<Param> getParams();
};

#endif