#include "ProcDecAST.hpp"

ProcDecAST::ProcDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_name = name;
	_params = params;
	check();
}

void ProcDecAST::check() {
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (name) {
		cerr << "Procedure name " << _name << " already exists." << endl;
	} else {
		vector< boost::shared_ptr<Param> > v = _params->getParams();
		vector< boost::shared_ptr<Param> >::iterator param;
		for (param=v.begin(); param < v.end(); param++) {
			_st->add((*param)->getName(), (*param)->getTypeName());
		}
		boost::shared_ptr<Proc> p(new Proc(_st, v));
		_procObj = p;
		_st->getEncSymTable()->add(_name, _procObj);
	}
}
