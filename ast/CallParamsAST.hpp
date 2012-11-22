#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

#include <antlr3commontree.h>
#include <vector>
#include "ASTNode.hpp"
#include "../idents/Type.hpp"
#include "../TreeUtils.hpp"

class CallParamsAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	pANTLR3_BASE_TREE _tree;
	boost::shared_ptr<ASTNode> _parent;
    int _lineNo;
	vector< boost::shared_ptr<Type> > _types;
	void check();
public:
	CallParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent, int lineNo);
	vector< boost::shared_ptr<Type> > getTypes();
};

#endif
