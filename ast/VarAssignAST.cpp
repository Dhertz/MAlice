#include "VarAssignAST.hpp"
#include "../idents/Identifier.hpp"

VarAssignAST::VarAssignAST(boost::shared_ptr<SymbolTable> st, string varName, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_varName = varName;
	_expr = expr;

	check();
}

void VarAssignAST::check() {
	// cout << "VarAssignAST::check() is looking for " << _varName << " in all levels of " << _st << "..." << endl;
 	boost::shared_ptr<Identifier> var = _st->lookupCurrLevelAndEnclosingLevels(_varName);

	// _st->printCurrLevelAndEnclosingLevels();

	if (!var) {
		cerr << "VarAssignAST: unknown variable " << _varName << endl;
	} else if (var->getBaseName() != "Variable") {
		cerr << _varName << " is not a variable" << endl;
	} else {
		boost::shared_ptr<Variable> varCasted = boost::shared_polymorphic_downcast<Variable>(var);

		// TODO: patch this up once all ExprAST::check() cases make the expression point to a type (i.e. not null)
		if (_expr->getTypeName()) {
			// _expr points to a type, so we can check it properly
			if (_expr->getTypeName()->getTypeName() != varCasted->getTypeName()->getTypeName()) {
				cerr << "lhs (" << varCasted->getTypeName()->getTypeName() << ") and rhs (" << _expr->getTypeName() << ") not type compatible" << endl;		
			} else {
				_varObj = varCasted;
			}
		} else {
			if (_expr->getTypeName() != varCasted->getTypeName()) {
				cerr << "lhs (" << varCasted->getTypeName()->getTypeName() << ") and rhs (" << _expr->getTypeName() << ") not type compatible" << endl;		
			} else {
				_varObj = varCasted;
			}
		}
	}	
}
