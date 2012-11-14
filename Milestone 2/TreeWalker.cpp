#include "TreeWalker.hpp"
#include "VariableDecAST.hpp"
#include "ExprAST.hpp"

//Construct object, assign fields and start walking the input tree from the top
TreeWalker::TreeWalker(SymbolTable* topSt, pANTLR3_BASE_TREE inputTree, 
					   AST* outputTree) {
	_topSt = topSt;
	_inputTree = inputTree;
	_outputTree = outputTree;

	walk(_inputTree, _topSt, NULL, 0);
}

// Call the correct method based on the type of the current AST node
void TreeWalker::walk(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
	string tokenName((const char *) tree->getText(tree)->chars, 
					 tree->getText(tree)->size);
	PROC tokenProcessor = _memberMap[tokenName];
	(this->*tokenProcessor)(tree, st, parent, childNum);
}

// Create the root of the tree and first non-static symbol table, go to children
void TreeWalker::processPROG(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
	/*
	SymbolTable progSt(st);

	ASTNode root(&progSt); // Maybe we need a different node for this?
	_outputTree->setRoot(&root);

	for (int i = 0; i < tree->getChildCount(tree); ++i) {
		walk(childByNum(tree, i), &progSt, &root, i);
	}
	*/
}

// Create the param list, get the id, create node and walk to body
void TreeWalker::processPROCDEC(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
	/*
	HeaderParamsAST params(st, tree->getChild(tree, 2));

	idTree = childByNum(tree, 1);
	string procName = createStringFromTree(idTree);

	ProcDecAST* dec(st, procName, params);

	parent->addChild(dec, childNum);

	walk(childByNum(tree, 3), st, dec, 3);
	*/
}

// We don't need a AST node here, but we do need a new symbol table as we have a new scope
void TreeWalker::processBODY(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
    SymbolTable scopeSt(st);

    for (int i = 0; i < tree->getChildCount(tree); ++i) {
        walk(childByNum(tree, i), &scopeSt, parent, i);
    }
}

// Like PROCDEC but with a type
void TreeWalker::processFUNCDEC(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
	/*
	HeaderParamsAST params(st, childByNum(tree, 2));

	idTree = (childByNum(tree, 1));
	typeTree = (childByNum(tree, 3));

	// Extract method from here
	string funcName = createStringFromTree(idTree);
	string funcType = createStringFromTree(typeTree);

	FuncDecAST dec(st, funcName, &params, funcType);

	parent->addChild(&dec, childNum);

	walk(childByNum(tree, 4), st, &dec, 4);
	*/
}

// Work out if it's an array or not, then process accordingly
void TreeWalker::processVARDEC(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
	pANTLR3_BASE_TREE idTree = childByNum(tree, 1);
	string varName = createStringFromTree(idTree);

	pANTLR3_BASE_TREE varOptionsTree = childByNum(tree, 2);
	string typeName = createStringFromTree(childByNum(varOptionsTree, 1));
	pANTLR3_BASE_TREE exprTree = childByNum(varOptionsTree, 2);

	if (createStringFromTree(varOptionsTree) == "NEWVAR") {
		if (exprTree == NULL) {
			VariableDecAST dec(st, typeName, varName);
			parent->addChild(&dec, childNum);
		} else {
			ExprAST expr(st, exprTree);
			VariableDecAST dec(st, typeName, varName, &expr);
			dec.addChild(&expr, 1);
			parent->addChild(&dec, childNum);
		}
	} else {
		/*
		ExprAST expr(st, exprTree);
		ArrayDecAST dec(st, expr, typeName);
		dec.addChild(&expr, 1);
		parent->addChild(&dec, childNum);
		*/
	}
}

void TreeWalker::processVARSTAT(pANTLR3_BASE_TREE tree, SymbolTable* st, ASTNode* parent, int childNum) {
	string varId = createStringFromTree(childByNum(tree, 1));

	optionsTree = childByNum(tree, 2);
	string option = createStringFromTree(optionsTree);

	if (option == "ARRMEMBER") {
		ExprAST elem(st, childByNum(optionsTree, 1));

		string subOption = createStringFromTree(childByNum(optionsTree, 2));
		if (subOption == "ate") {
			
		}
		// TODO
	} else if (option == "FUNC") {
		/*
		CallParamsAST params(st, childByNum(optionsTree, 1));
		FuncAST func(st, varId, &params);
		parent->addChild(FuncAST, childNum);
		*/
	} else if (option == "ASSIGN") {
		ExprAST expr(st, childByNum(optionsTree, 1));
		VarAssignAST assign(st, varId, &expr);
		parent->addChild(VarAssignAST, childNum);
	}
}

// Code shortening methods:

pANTLR3_BASE_TREE TreeWalker::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string TreeWalker::createStringFromTree(pANTLR3_BASE_TREE tree) {
	string res((const char *) tree->getText(tree)->chars, 
			   tree->getText(tree)->size);
	return res;
}

// map intialisation
void TreeWalker::init() {
	_memberMap["PROG"] 	    = &TreeWalker::processPROG;
	_memberMap["PROCDEC"]   = &TreeWalker::processPROCDEC;
	_memberMap["BODY"]		= &TreeWalker::processBODY;
	_memberMap["FUNCDEC"]   = &TreeWalker::processFUNCDEC;
	//_memberMap["HPL"] 	    = &TreeWalker::processHPL;
	_memberMap["VARDEC"]    = &TreeWalker::processVARDEC;
	//_memberMap["NEWVAR"]    = &TreeWalker::processNEWVAR;
	//_memberMap["NEWARR"]    = &TreeWalker::processNEWARR;
	_memberMap["VARSTAT"]   = &TreeWalker::processVARSTAT;
	//_memberMap["ARRMEMBER"] = &TreeWalker::processARRMEMBER;
	//_memberMap["FUNC"] 	    = &TreeWalker::processFUNC;
	//_memberMap["CPL"] 	    = &TreeWalker::processCPL;
	//_memberMap["ASSIGN"]    = &TreeWalker::processASSIGN;
	_memberMap["INC"]       = &TreeWalker::processINC;
	_memberMap["DEC"]       = &TreeWalker::processDEC;
	_memberMap["PRINT"]     = &TreeWalker::processPRINT;
	_memberMap["RETURN"]    = &TreeWalker::processRETURN;
	_memberMap["STDIN"]     = &TreeWalker::processSTDIN;
	_memberMap["WHILE"]     = &TreeWalker::processWHILE;
	_memberMap["CHOICE"]    = &TreeWalker::processCHOICE;
	_memberMap["IF"]        = &TreeWalker::processIF;
	_memberMap["COND"]      = &TreeWalker::processCOND;
	_memberMap["EXPR"]      = &TreeWalker::processEXPR;
}
