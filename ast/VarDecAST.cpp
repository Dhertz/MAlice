#include "VarDecAST.hpp"
#include "../Utils.hpp"

VarDecAST::VarDecAST(boost::shared_ptr<SymbolTable> st,
                                 string typeName, string varName,
                                 boost::weak_ptr<ASTNode> parent, int lineNo)
                                 : ASTNode(st, parent, lineNo) {
    _st = st;
    _typeName = typeName;
    _varName = varName;
    _lineNo = lineNo;
    check();
}

void VarDecAST::check() {
    boost::shared_ptr<Identifier> type =
      _st->lookupCurrLevelAndEnclosingLevels(_typeName);

    boost::shared_ptr<Identifier> name =
      _st->lookupCurrLevelOnly(_varName);

    if (!type) {
        Utils::printSemErr(_lineNo, "Variable type " + _typeName +
                             " is not in scope!");
    } else if (type->getBaseName() != "Type") {
        Utils::printSemErr(_lineNo, _typeName + " is not a type.");
    } else if (name) {
        Utils::printSemErr(_lineNo, _varName + " is already declared in this " +
                             "scope.");
    } else {
        boost::shared_ptr<Type> typeCasted =
          boost::shared_polymorphic_downcast<Type>(type);

        boost::shared_ptr<Variable> v(new Variable(typeCasted));
        _st->add(_varName, v);
    }
}

void VarDecAST::print() {
    cout << "Variable declaration node" << endl;
}

string VarDecAST::getNodeName() {
    return "VarDec";
}

void VarDecAST::accept(boost::shared_ptr<ASTVisitor> v, boost::shared_ptr<AssemFunc> func) {
  	v->visitVarDec(_typeName, _varName, _st, func);
}

void VarDecAST::accept(boost::shared_ptr<ASTVisitor> v) {
  	v->visitVarDec(_typeName, _varName, _st, boost::shared_ptr<AssemFunc>());
}
