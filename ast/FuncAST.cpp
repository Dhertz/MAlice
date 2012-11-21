#include "FuncAST.hpp"

FuncAST::FuncAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<CallParamsAST> params, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_name = name;
	_params = params;
	check();
}

void FuncAST::check() {
	boost::shared_ptr<Identifier> function = _st->lookupCurrLevelAndEnclosingLevels(_name);
	
	if (!function) {
		cerr << "Function " << _name << " not in scope." << endl;
	} else if (function->getBaseName() != "Callable") {
		cerr << _name << " is not a function, it's a " << function->getBaseName() << ". You can't call it." << endl;
	} else {
		boost::shared_ptr<Callable> funcCasted = boost::shared_polymorphic_downcast<Callable>(function);
		parametersTypeCheck(funcCasted);
	}
}

void FuncAST::parametersTypeCheck(boost::shared_ptr<Callable> function) {
	vector<boost::shared_ptr<Param> > params = function->getParams();
	vector<boost::shared_ptr<Param> >::iterator i = params.begin();
	
	vector<boost::shared_ptr<Type> > paramTypes = _params->getTypes();
	vector<boost::shared_ptr<Type> >::iterator j = paramTypes.begin();
	
	if (params.size() != paramTypes.size()) {
		cerr << "Invalid number of arguments for " << _name << " (expected " << paramTypes.size()
		<< ", got " << params.size() << ")." << endl;
	}
	for (; j != paramTypes.end(); ++j) {
		if(!(*i)) {
			cerr << "Bad parameter type." << endl;
		} else if ((*i)->getTypeName()->getBaseName() != (*j)->getBaseName()) {
			cerr << "Type mismatch for " << _name << "." << endl;
		}
		++i;
	}
}
