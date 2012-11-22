#ifndef HEADER_PARAMS_AST
#define HEADER_PARAMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>
#include <vector>
#include "../idents/Param.hpp"
#include "../idents/Spider.hpp"

class HeaderParamsAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	pANTLR3_BASE_TREE _tree;
	vector< boost::shared_ptr<Param> > _params;
public:
	HeaderParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent);
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	string createStringFromTree(pANTLR3_BASE_TREE tree);
	bool duplicate(int upto, string name);
	void check();
	vector< boost::shared_ptr<Param> > getParams();
};

#endif