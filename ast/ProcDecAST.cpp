#include "ProcDecAST.hpp"

ProcDecAST::ProcDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params) : ASTNode(st) {
	_st = st;
	_name = name;
	_params = params;
	check();
}

void ProcDecAST::check() {
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (name) {

	} else {
		vector< boost::shared_ptr<Param> > v = _params->getParams();
		vector< boost::shared_ptr<Param> >::iterator param;
		for (param=v.begin(); param < v.end(); param++) {
			_st->add((*param)->getName(), (*param)->getType());
		}
		boost::shared_ptr<Proc> p(new Proc(_st, v));
		_procObj = p;
		_st->getEncSymTable()->add(_name, _procObj);
	}
}
