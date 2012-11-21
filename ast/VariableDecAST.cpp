#include "VariableDecAST.hpp"
#include "../idents/Number.hpp"

VariableDecAST::VariableDecAST(boost::shared_ptr<SymbolTable> st, string typeName, string varName) : ASTNode(st) {
	_st = st;
	_typeName = typeName;
	_varName = varName;

	check();
}
void VariableDecAST::check() {
	// cout << "VariableDecAST::check() is looking for " << _typeName << " in all levels of " << _st << "..." << endl;
	boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(_typeName);

	// cout << "VariableDecAST::check() is looking for " << _varName << " in current level of " << _st << "..." << endl;
	boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(_varName);

	// _st->printCurrLevelAndEnclosingLevels();

	if (!type) {
		cerr << "unknown type " << _typeName << endl;
	} else if (type->getBaseName() != "Type") {
		cerr << _typeName  << " is not a type" << endl;
	} else if (name) {
		cerr << _varName << " is already declared" << endl;
	} else {
		cout << "Declaring variable " << _varName << endl;
		boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
		boost::shared_ptr<Variable> v(new Variable(typeCasted));
		_st->add(_varName, v);
	}
}
