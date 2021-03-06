#include "FuncProcCallAST.hpp"
#include <boost/lexical_cast.hpp>
#include "ProcDecAST.hpp"

FuncProcCallAST::FuncProcCallAST(boost::shared_ptr<SymbolTable> st, string name,
                   boost::shared_ptr<CallParamsAST> params,
                   boost::weak_ptr<ASTNode> parent, int lineNo)
                   : ASTNode(st, parent, lineNo) {
    _st = st;
    _name = name;
    _params = params;
    _lineNo = lineNo;
    check();
}

void FuncProcCallAST::check() {
	_name = checkHatta();
    boost::shared_ptr<Identifier> function =
      _st->lookupCurrLevelAndEnclosingLevels(_name);

    if (!function) {
        Utils::printSemErr(_lineNo, _name + " is not in scope!");
    } else if (function->getBaseName() != "Callable") {
        Utils::printSemErr(_lineNo, _name + " is not callable.");
    } else {
        boost::shared_ptr<Callable> funcCasted =
          boost::shared_polymorphic_downcast<Callable>(function);
        parametersTypeCheck(funcCasted);
    }
}

void FuncProcCallAST::parametersTypeCheck(boost::shared_ptr<Callable> function) {
    vector< boost::shared_ptr<Param> > params = function->getParams();
    vector< boost::shared_ptr<Param> >::iterator i = params.begin();

    vector< boost::shared_ptr<Type> > paramTypes = _params->getTypes();
    vector< boost::shared_ptr<Type> >::iterator j = paramTypes.begin();

    if (params.size() != paramTypes.size()) {
        string paramsStr = boost::lexical_cast<string>(params.size());
        string typesStr = boost::lexical_cast<string>(paramTypes.size());
        Utils::printSemErr(_lineNo, "Invalid number of arguments given to " +
                             _name + " (expected " + paramsStr + ", got " +
                             typesStr + ").");
    }

    int minimum = min(params.size(), paramTypes.size());
    for (int k = 0; k < minimum; ++i, ++j, ++k) {
        if (!(*i) || !(*i)->getType() || !(*j)) {
            Utils::printSemErr(_lineNo, (string) "Bad parameter type due to " +
                                 "an earlier error.");
        } else {
            string expType = (*i)->getType()->getTypeName();
            string actType = (*j)->getTypeName();

            if (expType != actType) {
                string argNum = boost::lexical_cast<string>(k + 1);
                Utils::printSemErr(_lineNo, "Argument " + argNum + " to " +
                                     _name + " should be a " + expType +
                                     ", not a " + actType + ".");
            }
        }
    }
}

string FuncProcCallAST::getNodeName() {
    return "FuncProcCall";
}

void FuncProcCallAST::print() {
    cout << "Function call" << endl;
}

void FuncProcCallAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
  	v->visitFuncCall(_name, _params, _st, func);
}

string FuncProcCallAST::checkHatta() {
	// make sure that hatta methods are named uniquely (makes life much simpler
	// in codegen)

	if (_name == "hatta") {
		boost::shared_ptr<Identifier> name =
	      _st->getEncSymTable()->lookupCurrLevelAndEnclosingLevels(_name);

	    if (name) {
	    	// nested function named hatta
	    	return "hatta" + boost::lexical_cast<string>(ProcDecAST::hattaNum);
	    }
	}
	return _name;
}
