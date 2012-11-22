#ifndef PROC_DEC_AST
#define PROC_DEC_AST

#include "ASTNode.hpp"
#include "HeaderParamsAST.hpp"
#include "../idents/Proc.hpp"
#include <vector>

class ProcDecAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	string _name;
	string _returnType;
	boost::shared_ptr<HeaderParamsAST> _params;
	boost::shared_ptr<Proc> _procObj;
    int _lineNo;
	void check();
public:
	ProcDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, boost::shared_ptr<ASTNode> parent, int lineNo);
};

#endif