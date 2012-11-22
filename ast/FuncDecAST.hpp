#ifndef FUNC_DEC_AST
#define FUNC_DEC_AST

#include "ASTNode.hpp"
#include "HeaderParamsAST.hpp"
#include "../idents/Function.hpp"
#include <vector>

class FuncDecAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	string _name;
	string _returnType;
	boost::shared_ptr<HeaderParamsAST> _params;
	boost::shared_ptr<Function> _funcObj;
	int _lineNo;
public:
	FuncDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, string returnType, boost::shared_ptr<ASTNode> parent, int lineNo);
	void check();
	string getNodeName();
	string getReturnType();
	string getFuncName();
};

#endif
