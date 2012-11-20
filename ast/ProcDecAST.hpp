#ifndef PROC_DEC_AST
#define PROC_DEC_AST

#include "HeaderParamsAST.hpp"

class ProcDecAST : public ASTNode {

public:
	ProcDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params);
};

#endif