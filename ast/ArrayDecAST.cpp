#include "ArrayDecAST.hpp"

ArrayDecAST::ArrayDecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> length, string name, string typeName, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_name = name;
	_length = length;
	_elemType = typeName;
	_lineNo = lineNo;
	check();
}

void ArrayDecAST::check() {
	boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(_elemType);
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(_name);
	
	if (!type) {
		cerr << "Line " << _lineNo << " - " << "Unknown identifier used as type." << endl;
	} else if (type->getBaseName() != "Type") {
		cerr << "Line " << _lineNo << " - " << "Not a type" << endl;
	} else if (name) {
		cerr << "Line " << _lineNo << " - " << "Variable already declared" << endl;
	} else if(!_length || !_length->getTypeName()) {
		cerr << "Line " << _lineNo << " - " << "Array " << _name << " has no length." << endl;

	// Charlie: is the double getTypeName safe here without casting to Type first?
	} else if ("Number" != _length->getTypeName()->getTypeName()) {
		cerr << "Line " << _lineNo << " - " << "Invalid array length";
	} else {
		boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
		boost::shared_ptr<Array> a(new Array(typeCasted));
		_arrObj = a;
		_st->add(_name, _arrObj);
	}
}
