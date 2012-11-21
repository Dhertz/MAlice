#include "FuncDecAST.hpp"

FuncDecAST::FuncDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, string returnType) : ASTNode(st) {
	_st = st;
	_name = name;
	_returnType = returnType;
	_params = params;
	check();
}

void FuncDecAST::check() {
	boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(_returnType);
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (!type) {

	} else if (type->getID() != "Type") {

	} else if (!name) {

	} else {
		vector< boost::shared_ptr<Param> > v = _params->getParams();
		vector< boost::shared_ptr<Param> >::iterator param;

		for (param = v.begin(); param < v.end(); param++) {
			_st->getEncSymTable()->add((*param)->getName(), (*param)->getType());
		}

		boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
		boost::shared_ptr<Function> f(new Function(typeCasted, v, _st));
		_funcObj = f;
		_st->add(_name, _funcObj);
	}
}
