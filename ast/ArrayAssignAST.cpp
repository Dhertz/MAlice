#include "ArrayAssignAST.hpp"
#include "../Utils.hpp"

ArrayAssignAST::ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name,
                                 boost::shared_ptr<ExprAST> element,
                                 boost::shared_ptr<ExprAST> value,
                                 boost::shared_ptr<ASTNode> parent,
                                 int lineNo) : ASTNode(st, parent, lineNo) {
    _st = st;
    _name = name;
    _element = element;
    _value = value;
    _lineNo = lineNo;
    check();
}

void ArrayAssignAST::check() {
    boost::shared_ptr<Identifier> array =
      _st->lookupCurrLevelAndEnclosingLevels(_name);

    if (!array) {
        Utils::printSemErr(_lineNo, "Array " + _name + " it is not in scope.");
    } else if (!_element || !_element->getType()) {
        Utils::printSemErr(_lineNo, (string) "Bad array index expression, " +
                             "due to an earlier error.");
    } else if (_element->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Array index must be a Number, not a " +
                             _element->getType()->getTypeName() + ".");
    } else if (array->getBaseName() != "Type") {
        Utils::printSemErr(_lineNo, _name + " is not an array!");
    } else {
        boost::shared_ptr<Type> arrayTypeCasted =
            boost::shared_polymorphic_downcast<Type>(array);
        if (arrayTypeCasted->getTypeName() != "Array") {
            Utils::printSemErr(_lineNo, _name + " is not an array!");
        } else {
            boost::shared_ptr<Array> arrCasted =
              boost::shared_polymorphic_downcast<Array>(array);

            if (!_value || !_value->getType()) {
                Utils::printSemErr(_lineNo, (string) "Bad array element " +
                                     "value expression, due to an earlier " +
                                     "error.");
            } else if (!arrCasted->getElemType()) {
                Utils::printSemErr(_lineNo, (string) "Bad array element " +
                                     "type, due to an earlier error.");
            } else if(_value->getType()->getTypeName() !=
                        arrCasted->getElemType()->getTypeName()) {
                Utils::printSemErr(_lineNo, _name + " elements must be a " +
                                     arrCasted->getElemType()->getTypeName() +
                                     ", not a " +
                                     _value->getType()->getTypeName() + ".");
            } else {
                _arrObj = arrCasted;
            }
        }
    }
}
