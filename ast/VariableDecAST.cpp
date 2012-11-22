#include "VariableDecAST.hpp"
#include <iostream>

VariableDecAST::VariableDecAST(boost::shared_ptr<SymbolTable> st, string typeName, string varName, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_typeName = typeName;
	_varName = varName;
	_lineNo = lineNo;
	check();
}
void VariableDecAST::check() {
	boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(_typeName);
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(_varName);

	if (!type) {
		cerr << "Line " << _lineNo << " - " << "Trying to declare variable of unknown type " << _typeName << "." << endl;
	} else if (type->getBaseName() != "Type") {
		cerr << "Line " << _lineNo << " - " << "Trying to declare variable of incorrect type " << _typeName << "." << endl;
	} else if (name) {
		cerr << "Line " << _lineNo << " - " << _varName << " is already declared." << endl;
	} else {
		boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
		boost::shared_ptr<Variable> v(new Variable(typeCasted));
		_st->add(_varName, v);
	}
}
