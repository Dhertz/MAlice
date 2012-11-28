#ifndef EXPR_AST_H
#define EXPR_AST_H

#include <antlr3commontree.h>
#include <set>
#include <vector>
#include "ASTNode.hpp"
#include "../idents/Type.hpp"

class ExprAST : public ASTNode {
    set<string> _boolArgBoolRet;
    set<string> _intArgIntRet;
    set<string> _boolArgsBoolRet;
    set<string> _mixedArgsMixedRet;
    set<string> _mixedArgsBoolRet;
    boost::shared_ptr<Type> _type;
    pANTLR3_BASE_TREE _tree;
    int _lineNo;
    bool _isAssignable;
	bool _hasExprTok;
    void check();
    boost::shared_ptr<Type> recurseTree(pANTLR3_BASE_TREE tree,
                                          string expectedType);
public:
    ExprAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree,
              boost::weak_ptr<ASTNode> parent, int lineNo, bool hasExprTok);
    boost::shared_ptr<Type> getType();
    bool isAssignable();
};

#endif
