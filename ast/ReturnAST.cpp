#include "ReturnAST.hpp"
#include "FuncDecAST.hpp"
#include "../idents/Type.hpp"

ReturnAST::ReturnAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
	_parent = parent;
	_lineNo = lineNo;
	check();
}

void ReturnAST::check() {
	if(!_expr) {
		cerr << "Line " << _lineNo << " - " << "Cannot return bad expression." << endl;
	}

	checkFunctionType(_parent);
}

void ReturnAST::checkFunctionType(boost::shared_ptr<ASTNode> parent) {
	if (!parent) {
		cerr << "Line " << _lineNo << " - " << "Null" << endl;
	}

	if (parent->getNodeName() == "FuncDec") {
		boost::shared_ptr<FuncDecAST> funcDec = boost::shared_polymorphic_downcast<FuncDecAST>(parent);
		// May need to be more rigerous here

		string funcName = funcDec->getFuncName();

		boost::shared_ptr<Function> func = boost::shared_polymorphic_downcast<Function>(_st->lookupCurrLevelAndEnclosingLevels(funcName));

		string funcReturnType = func->getTypeName()->getTypeName();
		string exprType = _expr->getTypeName()->getTypeName();

		if (funcReturnType != exprType) {
			cerr << "Line " << _lineNo << " - " << "Return type for function " << funcName << " (" << funcReturnType << ") does not match type of returned expression (" << exprType << ")." << endl;
		}
	} else {
		checkFunctionType(parent->getParent());
	}
}
