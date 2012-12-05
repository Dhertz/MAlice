#ifndef RETURN_AST
#define RETURN_AST

#include "ExprAST.hpp"
#include "../ASTVisitor.hpp"

class ReturnAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
    void check();
    void checkFunctionType(boost::weak_ptr<ASTNode> parent);
public:
    ReturnAST(boost::shared_ptr<SymbolTable> st,
    	        boost::shared_ptr<ExprAST> expr,
    	        boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
	void accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func);
};

#endif
