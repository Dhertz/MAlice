#ifndef ARRAY_DEC_AST
#define ARRAY_DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Array.hpp"

class ArrayDecAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ExprAST> _length;
	string _name;
	string _elemType;
	boost::shared_ptr<Array> _arrObj;
public:
	ArrayDecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, string name, string typeName, boost::shared_ptr<ASTNode> parent);
	void check();
};

#endif