#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

#include <antlr3commontree.h>
#include <vector>
#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Type.hpp"
#include "../Utils.hpp"

class CallParamsAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    pANTLR3_BASE_TREE _tree;
    boost::weak_ptr<ASTNode> _parent;
    int _lineNo;
    vector< boost::shared_ptr<Type> > _types;
    vector< boost::shared_ptr<ExprAST> > _params;
    void check();
public:
    CallParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree,
                    boost::weak_ptr<ASTNode> parent, int lineNo);
    vector< boost::shared_ptr<Type> > getTypes();
    vector< boost::shared_ptr<ExprAST> > getParamExprs();
    string getNodeName();
    void print();
};

#endif
