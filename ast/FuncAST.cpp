#include "FuncAST.hpp"

FuncAST::FuncAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<CallParamsAST> params, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = name;
	_params = params;
	_lineNo = lineNo;
	check();
}

void FuncAST::check() {
	boost::shared_ptr<Identifier> function = _st->lookupCurrLevelAndEnclosingLevels(_name);
	
	if (!function) {
		cerr << "Line " << _lineNo << " - " << "Function " << _name << " not in scope." << endl;
	} else if (function->getBaseName() != "Callable") {
		cerr << "Line " << _lineNo << " - " << _name << " is not a function, it's a " << function->getBaseName() << ". You can't call it." << endl;
	} else {
		boost::shared_ptr<Callable> funcCasted = boost::shared_polymorphic_downcast<Callable>(function);
		parametersTypeCheck(funcCasted);
	}
}

void FuncAST::parametersTypeCheck(boost::shared_ptr<Callable> function) {
	vector< boost::shared_ptr<Param> > params = function->getParams();
	vector< boost::shared_ptr<Param> >::iterator i = params.begin();
	
	vector< boost::shared_ptr<Type> > paramTypes = _params->getTypes();
	vector< boost::shared_ptr<Type> >::iterator j = paramTypes.begin();
	
	if (params.size() != paramTypes.size()) {
		cerr << "Invalid number of arguments for " << _name << "(expected " 
			<< params.size() << ", got " << paramTypes.size() << ")." << endl;
	}
	
	int minimum = min(params.size(), paramTypes.size());
	
	for (int k = 0; k < minimum; ++i, ++j, ++k) {
		if (!(*i)) {
			cerr << "Line " << _lineNo << " - " << "Bad parameter type." << endl;
		} else if (!(*j)) {
			cerr << "Line " << _lineNo << " - " << "Bad parameter type." << endl;
		} else if (!(*i)->getTypeName()) {
			cerr << "Line " << _lineNo << " - " << "Parameter has no type." << endl;
		} else if ((*i)->getTypeName()->getBaseName() != (*j)->getBaseName()) {
			cerr << "Line " << _lineNo << " - " << "Type mismatch for " << _name << "." << endl;
		}
	}
}
