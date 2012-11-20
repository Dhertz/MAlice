#include "FuncDecAST.hpp"

FuncDecAST::FuncDecAST(SymbolTable* st, string name, HeaderParamsAST* params, string returnType) : ASTNode(st) {
	_st = st;
	_name = name;
	_returnType = returnType;
	_params = params;
	check();
}

void FuncDecAST::check() {
	Identifier* type = _st->lookupCurrLevelAndEnclosingLevels(_returnType);
	Identifier* name = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (type == NULL) {

	} else if (type->getID() != "Type") {

	} else if (name != NULL) {

	} else {
		vector<Param> v = _params->getParams();
		vector<Param>::iterator param;
		for (param=v.begin(); param < v.end(); param++) {
			_st->getEncSymTable()->add(param->getName(), param->getType());
		}
		Function f((Type*) type, v, _st);
		_funcObj = &f;
		_st->add(_name, _funcObj);
	}
}