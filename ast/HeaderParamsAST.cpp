#include "HeaderParamsAST.hpp"

pANTLR3_BASE_TREE HeaderParamsAST::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string HeaderParamsAST::createStringFromTree(pANTLR3_BASE_TREE tree) {
	string res((const char *) tree->getText(tree)->to8(tree->getText(tree))->chars, 
			   tree->getText(tree)->len);
	return res;
}

HeaderParamsAST::HeaderParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree) : ASTNode(st) {
	_st = st;
	_tree = tree;

	check();
}


void HeaderParamsAST::check() {
	for (int i = 0; i < _tree->getChildCount(_tree); i=2+i) {
		
		string typeString = createStringFromTree(childByNum(_tree, i));
		string nameString = createStringFromTree(childByNum(_tree, i+1));
		Identifier* type = _st->lookupCurrLevelAndEnclosingLevels(typeString);
		Identifier* name = _st->lookupCurrLevelOnly(nameString);
		
		if (type->getID() != "Type") {

		} else if(name != NULL) {

		} else if(duplicate(i+1, nameString)) {

		} else {
			Param p((Type*) type, nameString);
			_params.push_back(p);
		}
	}
}

vector<Param> HeaderParamsAST::getParams() {
	return _params;
}

bool HeaderParamsAST::duplicate(int upto, string name) {
	for (int i = 0; i < upto; i=i+2) {
		string nameString = createStringFromTree(childByNum(_tree, i+1));
		if (nameString == name) {
			return true;
		}
	}
	return false;
}
