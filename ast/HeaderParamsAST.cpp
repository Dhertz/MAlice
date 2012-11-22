#include "HeaderParamsAST.hpp"
#include "../idents/Array.hpp"

HeaderParamsAST::HeaderParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_tree = tree;
	_lineNo = lineNo;
	check();
}


void HeaderParamsAST::check() {
	for (int i = 0; i + 1 < _tree->getChildCount(_tree); i += 2) {

		string typeString = TreeUtils::createStringFromTree(TreeUtils::childByNum(_tree, i));
		if (typeString == "spider") {
			++i;
			typeString = TreeUtils::createStringFromTree(TreeUtils::childByNum(_tree, i));
			string nameString = TreeUtils::createStringFromTree(TreeUtils::childByNum(_tree, i+1));
			boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(typeString);
			boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(nameString);
			if(!type) {
				cerr << "Line " << _lineNo << " - " << "Header type " << typeString << " doesn't exist." << endl;
			} else if (type->getBaseName() != "Type") {
				cerr << "Line " << _lineNo << " - " << "Can't have a " << typeString << " parameter." << endl;
			} else if (name) {
				cerr << "Line " << _lineNo << " - " << nameString << " has already been declared." << endl;
			} else if (duplicate(i, nameString)) {
				cerr << "Line " << _lineNo << " - " << "Duplicate parameters " << nameString << "." << endl;
			} else {
				boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
				boost::shared_ptr<Type> array(new Array(typeCasted));
				boost::shared_ptr<Param> p(new Param(array, nameString));
				_params.push_back(p);
			}
		} else {
			string nameString = TreeUtils::createStringFromTree(TreeUtils::childByNum(_tree, i+1));
			boost::shared_ptr<Identifier> type = _st->lookupCurrLevelAndEnclosingLevels(typeString);
			boost::shared_ptr<Identifier> name = _st->lookupCurrLevelOnly(nameString);
			if(!type) {
				cerr << "Line " << _lineNo << " - " << "Header type " << typeString << " doesn't exist." << endl;
			} else if (type->getBaseName() != "Type") {
				cerr << "Line " << _lineNo << " - " << "Can't have a " << typeString << " parameter." << endl;
			} else if (name) {
				cerr << "Line " << _lineNo << " - " << nameString << " has already been declared." << endl;
			} else if (duplicate(i, nameString)) {
				cerr << "Line " << _lineNo << " - " << "Duplicate parameters " << nameString << "." << endl;
			} else {
				boost::shared_ptr<Type> typeCasted = boost::shared_polymorphic_downcast<Type>(type);
				boost::shared_ptr<Param> p(new Param(typeCasted, nameString));
				_params.push_back(p);
			}
		}
	}
}

vector< boost::shared_ptr<Param> > HeaderParamsAST::getParams() {
	return _params;
}

bool HeaderParamsAST::duplicate(int upto, string name) {
	for (int i = 0; i+1 < upto; i+=2) {
		string nameString = TreeUtils::createStringFromTree(TreeUtils::childByNum(_tree, i+1));
		if(nameString == "spider") {
			i++;
			nameString = TreeUtils::createStringFromTree(TreeUtils::childByNum(_tree, i+1));
		}
		if (nameString == name) {
			return true;
		}
	}
	return false;
}
