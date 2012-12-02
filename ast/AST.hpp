#ifndef AST_H
#define AST_H

#include "ASTNode.hpp"
#include "ProgAST.hpp"

class AST {
    boost::shared_ptr<ProgAST> _root;
public:
    void setRoot(boost::shared_ptr<ProgAST> root);
    boost::shared_ptr<ProgAST> getRoot();
    void print();
    void printNode(boost::shared_ptr<ASTNode> node, int level);
};

#endif
