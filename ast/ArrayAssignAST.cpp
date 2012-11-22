#include "ArrayAssignAST.hpp"

ArrayAssignAST::ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<ExprAST> element, boost::shared_ptr<ExprAST> value, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = name;
	_element = element;
	_value = value;
	_lineNo = lineNo;
	check();
}

void ArrayAssignAST::check() {
	boost::shared_ptr<Identifier> array = _st->lookupCurrLevelAndEnclosingLevels(_name);
	
	if (!array) {
		cerr << "Line " << _lineNo << " - " << "Can't assign to " << _name << " as its not in scope." << endl;
	} else if (_element->getTypeName()->getTypeName() != "Number") {
		cerr << "Line " << _lineNo << " - " << "Not a valid element number." << endl;
	} else if (!array) {
		cerr << "Line " << _lineNo << " - " << "Unknown variable " << _name << endl;
	} else if (array->getBaseName() != "Type") {
		cerr << "Line " << _lineNo << " - " << "Attempted array assignment on " << _name << " which is not an array." << endl;
	} else {
		boost::shared_ptr<Type> arrayTypeCasted = boost::shared_polymorphic_downcast<Type>(array);
		if (arrayTypeCasted->getTypeName() != "Array") {
			cerr << "Line " << _lineNo << " - " << "Attempted array assignment on " << _name << " which is not an array." << endl;
		} else {
			boost::shared_ptr<Array> arrCasted = boost::shared_polymorphic_downcast<Array>(array);

			if(_value->getTypeName()->getTypeName() != arrCasted->getElemType()->getTypeName()) {
				cerr << "Line " << _lineNo << " - " << "Type error. (" << _value->getTypeName()->getTypeName() << " != " << arrCasted->getElemType()->getTypeName() << ")" << endl;
			} else {
				_arrObj = arrCasted;
			}
		}
	}
}
