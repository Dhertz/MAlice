#include "ProcDecAST.hpp"
#include "../idents/Variable.hpp"

ProcDecAST::ProcDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = name;
	_params = params;
	_lineNo = lineNo;
	check();
}

void ProcDecAST::check() {
	// Needs checking (may have to be more levels)
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(_name);

	if (name) {
		cerr << "Line " << _lineNo << " - " << "Procedure name " << _name << " already exists." << endl;
	} else {
		vector< boost::shared_ptr<Param> > v = _params->getParams();
		vector< boost::shared_ptr<Param> >::iterator param;

		for (param=v.begin(); param != v.end(); param++) {
			if((*param)->getTypeName()->getTypeName() == "Array") {
				// Charlie: old version, in case the below shortcut ends up breaking
				// Basically an array is already created in HeaderParamsAST, so there's no point making a new one here

				// boost::shared_ptr<Type> paramType = (*param)->getTypeName();
				// boost::shared_ptr<Array> paramArray = boost::shared_polymorphic_downcast<Array>(paramType);
				// boost::shared_ptr<Identifier> arr(new Array(paramArray->getElemType()));

				_st->add((*param)->getName(), (*param)->getTypeName());
			} else {
				boost::shared_ptr<Identifier> var(new Variable((*param)->getTypeName()));
				_st->add((*param)->getName(), var);
			}
		}

		boost::shared_ptr<Proc> p(new Proc(_st, v));
		_procObj = p;
		_st->getEncSymTable()->add(_name, _procObj);
	}
}
