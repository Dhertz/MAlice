#ifndef AST_NODE_H
#define AST_NODE_H

#include <vector>
#include "../SymbolTable.hpp"

class ASTNode {
    vector< boost::shared_ptr<ASTNode> > _children;
    virtual void check();
protected:
    boost::shared_ptr<SymbolTable> _st;
    boost::weak_ptr<ASTNode> _parent;
    int _lineNo;
public:
    ASTNode(boost::shared_ptr<SymbolTable> st,
              boost::weak_ptr<ASTNode> parent, int lineNo);
    vector< boost::shared_ptr<ASTNode> > getChildren();
    void addChild(boost::shared_ptr<ASTNode> child, int position);
    boost::weak_ptr<ASTNode> getParent();
    virtual string getNodeName();
    virtual void print();
};

#endif
