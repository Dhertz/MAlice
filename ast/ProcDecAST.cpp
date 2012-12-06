#include "ProcDecAST.hpp"
#include "../idents/Variable.hpp"
#include <boost/lexical_cast.hpp>

ProcDecAST::ProcDecAST(boost::shared_ptr<SymbolTable> st, string name,
                         boost::shared_ptr<HeaderParamsAST> params,
                         boost::weak_ptr<ASTNode> parent, int lineNo)
                         : ASTNode(st, parent, lineNo) {
    _st = st;
    _name = name;
    _params = params;
    _lineNo = lineNo;
    check();
}

int ProcDecAST::hattaNum = 1;

void ProcDecAST::check() {
    boost::shared_ptr<Identifier> name =
      _st->getEncSymTable()->lookupCurrLevelOnly(_name);

    if (name) {
        Utils::printSemErr(_lineNo, _name + " has already been defined in " +
                             "this scope.");
    } else {
        vector< boost::shared_ptr<Param> > v = _params->getParams();
        vector< boost::shared_ptr<Param> >::iterator param;

        for (param=v.begin(); param != v.end(); ++param) {
            // No need to do null checks here since that is covered in
            //   HeaderParamsAST
            if((*param)->getType()->getTypeName() == "Array") {
                _st->add((*param)->getName(), (*param)->getType());
            } else {
                boost::shared_ptr<Identifier> var(
                  new Variable((*param)->getType())
                );
                _st->add((*param)->getName(), var);
            }
        }

        _name = checkHatta();

        boost::shared_ptr<Proc> p(new Proc(_st, v));
        _procObj = p;
        _st->getEncSymTable()->add(_name, _procObj);
    }
}

string ProcDecAST::getProcName() {
    return _name;
}

string ProcDecAST::getNodeName() {
    return "ProcDec";
}

void ProcDecAST::print() {
    cout << "Procedure declaration" << endl;
}

void ProcDecAST::accept(boost::shared_ptr<ASTVisitor> v) {
    v->visitProcDec(_name, _params, _children, _st);
}

string ProcDecAST::checkHatta() {
	// make sure that hatta methods are named uniquely (makes life much simpler
	// in codegen)

	if (_name == "hatta") {
		boost::shared_ptr<Identifier> name =
	      _st->getEncSymTable()->lookupCurrLevelAndEnclosingLevels(_name);

	    if (name) {
	    	// nested function named hatta
	    	hattaNum++;
	    	return "hatta" + boost::lexical_cast<string>(hattaNum);
	    }
	}
	return _name;
}
