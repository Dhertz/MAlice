#include "ArrayAssignAST.hpp"

ArrayAssignAST::ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<ExprAST> element, boost::shared_ptr<ExprAST> value) : ASTNode(st) {
	_st = st;
	_name = name;
	_element = element;
	_value = value;
	check();
}

void ArrayAssignAST::check() {
	boost::shared_ptr<Identifier> array = _st->lookupCurrLevelAndEnclosingLevels(_name);
	
	if (!_name) {
		cerr << _name << "not in scope!" << endl;
	} else if(_element->getType()->getID() != "Number") {
		cerr << "Not a valid element number." << endl;
	} else if (!array) {
		cerr << "Unknown variable " << _name << endl;
	} else if(array->getID() != "Array") {
		cerr << "Attempted array assignment on object which is not an array." << endl;
	} else {
		boost::shared_ptr<Array> arrCasted = boost::shared_polymorphic_downcast<Array>(array);

		if(_value->getType()->getID() == arrCasted->getElemType()->getID()) {
			cerr << "Type error." << endl;
		} else {
			_arrObj = arrCasted;
		}
	}
}
