#include "ReturnAST.hpp"
#include "FuncDecAST.hpp"

ReturnAST::ReturnAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
	_parent = parent;

	check();
}

void ReturnAST::check() {
	if(!_expr) {
		cerr << "Cannot return bad expression." << endl;
	}

	checkFunctionType(_parent);
}

void ReturnAST::checkFunctionType(boost::shared_ptr<ASTNode> parent) {
	if (!parent) {
		cerr << "Null" << endl;
	}

	if (parent->getNodeName() == "FuncDec") {
		boost::shared_ptr<FuncDecAST> funcDec = boost::shared_polymorphic_downcast<FuncDecAST>(parent);
		// May need to be more rigerous here

		string funcName = funcDec->getFuncName();

		boost::shared_ptr<Function> func = boost::shared_polymorphic_downcast<Function>(_st->lookupCurrLevelAndEnclosingLevels(funcName));

		string funcReturnType = func->getTypeName()->getTypeName();
		string exprType = _expr->getTypeName()->getTypeName();

		if (funcReturnType != exprType) {
			cerr << "Return type for function " << funcName << " (" << funcReturnType << ") does not match type of returned expression (" << exprType << ")." << endl;
		}
	} else {
		checkFunctionType(parent->getParent());
	}
}
