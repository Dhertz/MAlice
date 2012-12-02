#include "FuncDecAST.hpp"
#include "../idents/Array.hpp"
#include "../idents/Variable.hpp"

FuncDecAST::FuncDecAST(boost::shared_ptr<SymbolTable> st, string name,
                         boost::shared_ptr<HeaderParamsAST> params,
                         string returnType, boost::weak_ptr<ASTNode> parent,
                         int lineNo) : ASTNode(st, parent, lineNo) {
    _st = st;
    _name = name;
    _returnType = returnType;
    _params = params;
    _lineNo = lineNo;
    check();
}

void FuncDecAST::check() {
    boost::shared_ptr<Identifier> type =
      _st->lookupCurrLevelAndEnclosingLevels(_returnType);

    boost::shared_ptr<Identifier> name =
      _st->getEncSymTable()->lookupCurrLevelOnly(_name);

    if (!type) {
        Utils::printSemErr(_lineNo, "Return type " + _returnType +
                             " is not in scope!");
    } else if (type->getBaseName() != "Type") {
        Utils::printSemErr(_lineNo, "Return type " + _returnType +
                             " is not a type.");
    } else if (name) {
        Utils::printSemErr(_lineNo, _name + " has already been declared.");
    } else {
        vector< boost::shared_ptr<Param> > v = _params->getParams();
        vector< boost::shared_ptr<Param> >::iterator param;

        for (param = v.begin(); param != v.end(); ++param) {
            // No need to do null checks here since that is covered in
            //   HeaderParamsAST
            if ((*param)->getType()->getTypeName() == "Array") {
                boost::shared_ptr<Identifier> arr(
                  new Array((*param)->getType())
                );
                _st->add((*param)->getName(), arr);
            } else {
                boost::shared_ptr<Identifier> var(
                  new Variable((*param)->getType())
                );
                _st->add((*param)->getName(), var);
            }
        }

        boost::shared_ptr<Type> typeCasted =
          boost::shared_polymorphic_downcast<Type>(type);
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

void FuncDecAST::print() {
  cout << "Function declaration" << endl;
}

void FuncDecAST::accept(boost::shared_ptr<ASTVisitor> v) {
  v->visitFuncDec(_name, _returnType, _params, _children, _st);
}