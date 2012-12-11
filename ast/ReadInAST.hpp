#ifndef READ_IN_AST
#define READ_IN_AST

#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class ReadInAST : public ASTNode {
    boost::shared_ptr<SymbolTable> st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
public:
    ReadInAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    	       boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
