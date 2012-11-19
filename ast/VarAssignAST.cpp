#include "VarAssignAST.hpp"
#include "../idents/Identifier.hpp"

VarAssignAST::VarAssignAST(SymbolTable* st, string varName, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_varName = varName;
	_expr = expr;

	check();
}

void VarAssignAST::check() {
	Identifier* var = _st->lookupCurrLevelAndEnclosingLevels(_varName);

	cout << "VarAssignAST::check() is looking for " << _varName << " in all levels of " << _st << "..." << endl;
	_st->printCurrLevelAndEnclosingLevels();

	if (var == NULL) {
		cerr << "unknown variable " << _varName << endl;
	} else if (var->getID() != "Variable") {
		cerr << _varName << " is not a variable" << endl;
	} else {
		Variable* varCasted = (Variable*) var;

		if (_expr->getType() != varCasted->getType()) {
			cerr << "lhs and rhs not type compatible" << endl;	
		} else {
			cout << "Sweet." << endl;
			_varObj = varCasted;
		}
	}	
}
