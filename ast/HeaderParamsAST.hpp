#ifndef HEADER_PARAMS_AST
#define HEADER_PARAMS_AST

#include "ASTNode.hpp"
#include "../idents/Param.hpp"
#include "../Utils.hpp"

class HeaderParamsAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    pANTLR3_BASE_TREE _tree;
    vector< boost::shared_ptr<Param> > _params;
    int _lineNo;
    void check();
    bool duplicate(int upto, string name);
public:
    HeaderParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree,
    	              boost::shared_ptr<ASTNode> parent, int lineNo);
    vector< boost::shared_ptr<Param> > getParams();
};

#endif