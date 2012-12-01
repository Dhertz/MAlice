#ifndef ARRAY_ASSIGN_AST
#define ARRAY_ASSIGN_AST

#include "ExprAST.hpp"
#include "../idents/Array.hpp"
#include "../ASTVisitor.hpp"

class ArrayAssignAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _name;
    boost::shared_ptr<ExprAST> _element;
    boost::shared_ptr<ExprAST> _value;
    boost::shared_ptr<Array> _arrObj;
    int _lineNo;
    void check();
public:
    ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name,
        boost::shared_ptr<ExprAST> element, boost::shared_ptr<ExprAST> value,
        boost::weak_ptr<ASTNode> parent, int lineNo);
    void print();
    string getNodeName();
	void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
