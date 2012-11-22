#include "FuncDecAST.hpp"
#include "../idents/Variable.hpp"

FuncDecAST::FuncDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, string returnType, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = name;
	_returnType = returnType;
	_params = params;
	_lineNo = lineNo;
	check();
}

void FuncDecAST::check() {
	boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(_returnType);
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelAndEnclosingLevels(_name);

	if (!type) {
		cerr << "Line " << _lineNo << " - " << "Return type " << _returnType << " not known" << "." << endl;
	} else if (type->getBaseName() != "Type") {
		cerr << "Line " << _lineNo << " - " << "Return type " << _returnType << " is not a type." << endl;
	} else if (name) {
		cerr << "Line " << _lineNo << " - " << "Function " << _name << " already exists" << endl;
	} else {
		vector< boost::shared_ptr<Param> > v = _params->getParams();
		vector< boost::shared_ptr<Param> >::iterator param;

		for (param=v.begin(); param != v.end(); param++) {
			if((*param)->getTypeName()->getTypeName() == "Array") {
				boost::shared_ptr<Identifier> arr(new Array((*param)->getTypeName()));
				_st->add((*param)->getName(), arr);
			} else {
				boost::shared_ptr<Identifier> var(new Variable((*param)->getTypeName()));
				_st->add((*param)->getName(), var);
			}
		}

		boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
		boost::shared_ptr<Function> f(new Function(_st, v, typeCasted));
		_funcObj = f;
		_st->getEncSymTable()->add(_name, _funcObj);
	}
}

string FuncDecAST::getNodeName() {
	return "FuncDec";
}

string FuncDecAST::getFuncName() {
	return _name;
}
