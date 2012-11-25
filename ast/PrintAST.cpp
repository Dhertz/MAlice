#include "PrintAST.hpp"

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, boost::shared_ptr<ExprAST> element, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = arrayName;
	_element = element;
	_lineNo = lineNo;

	boost::shared_ptr<Identifier> object = _st->lookupCurrLevelAndEnclosingLevels(_name);
	if (!object) {
		cerr << "Line " << _lineNo << " - " << "Cannot print " << _name << " as it doesn't exist." << endl;
	} else {
		_type = object;
	}
}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, boost::shared_ptr<CallParamsAST> params, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = funcName;
	_params = params;
	_lineNo = lineNo;

	boost::shared_ptr<Identifier> func = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (!func) {
		cerr << "Line " << _lineNo << " - "<< "Cannot print " << _name << " as it doesn't exist." << endl;
	} else if (!_params) {
		cerr << "Line " << _lineNo << " - " << "Cannot print " << _name << " as it has no parameters." << endl;
	} else {
		_type = func;
	}
}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	if (!expr || !expr->getTypeName()) {
		cerr << "Line " << _lineNo << " - " << "Cannot print bad expression." << endl;
	} else if (expr->getTypeName()->getTypeName() == "Array") {
		cerr << "Line " << _lineNo << " - " << "Cannot print an array." << endl;
	} else {
		_expr = expr;
	}
}

boost::shared_ptr<Identifier> PrintAST::getTypeName() {
	return _type;
}

boost::shared_ptr<ExprAST> PrintAST::getExpr() {
	return _expr;
}
