#ifndef ARRAY_ASSIGN_AST
#define ARRAY_ASSIGN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Array.hpp"

class ArrayAssignAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	string _name;
	boost::shared_ptr<ExprAST> _element;
	boost::shared_ptr<ExprAST> _value;
	boost::shared_ptr<Array> _arrObj;
    int _lineNo;
public:
	ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<ExprAST> element, boost::shared_ptr<ExprAST> value, boost::shared_ptr<ASTNode> parent, int lineNo);
	void check();
};

#endif