#include "VarAssignAST.hpp"

VarAssignAST::VarAssignAST(SymbolTable* st, string varName, ExprAST expr) {
	_st = st;
	_varName = varName;
	_expr = expr;
}

Type VarAssignAST::check() {
	Identifier* var = _st->lookupCurrLevelAndEnclosingLevels(_varName);
	Type t = _expr.check();
	if(t.getID() != "Expr") {
		cout << t.getID() << "is not an expression!" << endl;
		NullType null;
		return null;
	} else if(var == NULL) {
		cout << "Variable name" << _varName << "doesn't exist!" << endl;
		NullType null;
		return null;
	} else if(var->getID() != "Variable") {
		cout << var->getID() << "is not a variable!" << endl;
		NullType null;
		return null;
	} else if(typeid(((Variable&) *var).getType()) != typeid(t)) {
		cout << "lhs and rhs not compatable" << endl;
		NullType null;
		return null;
	} else {
		_varObj = ((Variable&) *var);
		return ((Variable&) *var).getType();
	}
}