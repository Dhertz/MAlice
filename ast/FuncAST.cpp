#include "FuncAST.hpp"

FuncAST::FuncAST(SymbolTable* st, string name, CallParamsAST* params) : ASTNode(st) {
	_st = st;
	_name = name;
	_params = params;
	check();
}

void FuncAST::check() {
	Identifier* function = _st->lookupCurrLevelOnly(_name);
	if (function == NULL) {

	} else if (function->getID() != "Function") {

	} else if (!parametersTypeCheck((Function*) function)) {

	}
}

bool FuncAST::parametersTypeCheck(Function* function) {
	
	vector<Param> params = function->getParams();
	vector<Param>::iterator i = params.begin();
	
	vector<Type*> paramTypes = _params->getTypes();
	vector<Type*>::iterator j = paramTypes.begin();
	
	if (params.size() != paramTypes.size()) {
		cerr << "Invalid number of arguements for " << _name << endl;
		return false;
	}

	for (; j != paramTypes.end(); ++j) {
		if (i->getType()->getID() != (*j)->getID()) {
			cerr << "Type mismatch for " << _name << endl;
			return false;
		} else {
			++i;
		}
	}

}