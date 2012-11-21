#include "ArrayDecAST.hpp"

ArrayDecAST::ArrayDecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> length, string name, string typeName) : ASTNode(st) {
	_st = st;
	_name = name;
	_length = length;
	_elemType = typeName;
	check();
}

void ArrayDecAST::check() {
	boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(_elemType);
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(_name);
	
	if (!type) {
		cerr << "Unknown identifier used as type." << endl;
	} else if (type->getBaseName() != "Type") {
		cerr << "Not a type" << endl;
	} else if (name) {
		cerr << "Variable already declared" << endl;
	} else if ("Number" != _length->getTypeName()->getTypeName()) {
		cerr << "Invalid array length";
	} else {
		boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
		boost::shared_ptr<Array> a(new Array(typeCasted));
		_arrObj = a;
		_st->add(_name, _arrObj);
	}
}
