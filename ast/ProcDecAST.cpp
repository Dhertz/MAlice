#include "ProcDecAST.hpp"
#include "../idents/Variable.hpp"

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
    v->visitPROCDEC(_name, _params);
}
