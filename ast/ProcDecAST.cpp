#include "ProcDecAST.hpp"

ProcDecAST::ProcDecAST(SymbolTable* st, string name, HeaderParamsAST* params) : ASTNode(st) {
	_st = st;
	_name = name;
	_params = params;
	check();
}

void ProcDecAST::check() {
	Identifier* name = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (name != NULL) {

	} else {
		vector<Param> v = _params->getParams();
		vector<Param>::iterator param;
		for (param=v.begin(); param < v.end(); param++) {
			_st->add(param->getName(), param->getType());
		}
		Proc p(v, _st);
		_procObj = &p;
		_st->getEncSymTable()->add(_name, _procObj);
	}
}