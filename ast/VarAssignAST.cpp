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
	} else if (var->getID() != "Variable") {
		cerr << _varName << " is not a variable" << endl;
	} else {
		boost::shared_ptr<Variable> varCasted = boost::shared_polymorphic_downcast<Variable>(var);

		// TODO: replace getType() with getType()->getType() once ExprAST::check() has been implemented and sets its type
		if (_expr->getType() != varCasted->getType()) {
			cerr << "lhs (" << varCasted->getType()->getType() << ") and rhs (" << _expr->getType() << ") not type compatible" << endl;	
		} else {
			_varObj = varCasted;
		}
	}	
}
