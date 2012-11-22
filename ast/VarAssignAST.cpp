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
		cerr << "Line " << _lineNo << " - " << "Cannot assign to non-exisitant variable " << _varName  << "." << endl;
	} else if (var->getBaseName() != "Variable") {
		cerr << "Line " << _lineNo << " - " << "Cannot declare to " << _varName << " as it is a" 
			<< var->getBaseName() << "." << endl;
	} else {
		boost::shared_ptr<Variable> varCasted = boost::shared_polymorphic_downcast<Variable>(var);

		// TODO: patch this up once all ExprAST::check() cases make the expression point to a type (i.e. not null)
		if(!_expr) {
			cerr << "Line " << _lineNo << " - " << "Cannot assign variable" << _varName << "to bad expression." << endl;
		} else if (_expr->getTypeName()) {
			// _expr points to a type, so we can check it properly
			if (_expr->getTypeName()->getTypeName() != varCasted->getTypeName()->getTypeName()) {
				cerr << "Line " << _lineNo << " - " << "lhs (" << varCasted->getTypeName()->getTypeName() << ") and rhs (" << _expr->getTypeName()->getTypeName() << ") not type compatible" << endl;		
			} else {
				_varObj = varCasted;
			}
		}
	}	
}
