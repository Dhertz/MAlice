#ifndef IF_AST
#define IF_AST

#include "../ASTVisitor.hpp"
#include "IfBodyAST.hpp"
#include "ExprAST.hpp"

class IfAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    boost::shared_ptr<IfBodyAST> _body;
    int _lineNo;
    void check();
public:
    IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr,
    		boost::shared_ptr<IfBodyAST> body,
    	    boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
    void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
