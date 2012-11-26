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
    boost::shared_ptr<Identifier> name = _st->getEncSymTable()->lookupCurrLevelOnly(_name);

    if (name) {
        cerr << "Line " << _lineNo << " - " << "Procedure name " << _name << " already exists." << endl;
    } else {
        vector< boost::shared_ptr<Param> > v = _params->getParams();
        vector< boost::shared_ptr<Param> >::iterator param;

        for (param=v.begin(); param != v.end(); param++) {
            // No need to do null checks here since that is covered in HeaderParamsAST
            if((*param)->getTypeName()->getTypeName() == "Array") {
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
