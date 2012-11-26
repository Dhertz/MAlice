#include "VarAssignAST.hpp"
#include <iostream>

VarAssignAST::VarAssignAST(boost::shared_ptr<SymbolTable> st, string varName, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_varName = varName;
	_expr = expr;
	_lineNo = lineNo;
	check();
}

void VarAssignAST::check() {
 	boost::shared_ptr<Identifier> var = _st->lookupCurrLevelAndEnclosingLevels(_varName);

	if (!var) {
		cerr << "Line " << _lineNo << " - " << "Cannot assign to non-exisitent variable " << _varName  << "." << endl;
	} else if (var->getBaseName() != "Variable") {
		cerr << "Line " << _lineNo << " - " << "Cannot declare to " << _varName << " as it is a " 
			<< var->getBaseName() << ", which is not an r-value" << endl;
	} else {
		boost::shared_ptr<Variable> varCasted = boost::shared_polymorphic_downcast<Variable>(var);

		if(!_expr || !_expr->getType()) {
			cerr << "Line " << _lineNo << " - " << "Cannot assign variable" << _varName << "to bad expression." << endl;
		} else if (!varCasted->getTypeName()) {
			cerr << "Line " << _lineNo << " - " << "Variable " << _varName << " has bad type" << endl;
		} else if (_expr->getType()->getTypeName() != varCasted->getTypeName()->getTypeName()) {
				cerr << "Line " << _lineNo << " - " << "lhs (" << varCasted->getTypeName()->getTypeName() << ") and rhs (" << _expr->getType()->getTypeName() << ") not type compatible" << endl;		
		} else {
				_varObj = varCasted;
		}
	}	
}
