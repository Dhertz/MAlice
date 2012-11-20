// These includes are well ugly...
#include "TreeWalker.hpp"
#include "ast/VariableDecAST.hpp"
#include "ast/ExprAST.hpp"
#include "ast/ArrayDecAST.hpp"
#include "ast/ArrayAssignAST.hpp"
#include "ast/VarAssignAST.hpp"
#include "ast/FuncAST.hpp"
#include "ast/CallParamsAST.hpp"
#include "ast/HeaderParamsAST.hpp"
#include "ast/ProcDecAST.hpp"
#include "ast/FuncDecAST.hpp"
#include "ast/IncAST.hpp"
#include "ast/DecAST.hpp"
#include "ast/PrintAST.hpp"
#include "ast/ReturnAST.hpp"
#include "ast/StdinAST.hpp"
#include "ast/WhileAST.hpp"
#include "ast/ChoiceAST.hpp"
#include "ast/IfAST.hpp"

//Construct object, assign fields and start walking the input tree from the top
TreeWalker::TreeWalker(boost::shared_ptr<SymbolTable> topSt, pANTLR3_BASE_TREE inputTree, 
					   AST* outputTree) {
	_topSt = topSt;
	_inputTree = inputTree;
	_outputTree = outputTree;

	init();

	walk(_inputTree, _topSt, NULL, 0);
}

// Call the correct method based on the type of the current AST node
void TreeWalker::walk(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	string tokenName = createStringFromTree(tree);
	map<string, PROC>::const_iterator it = _memberMap.find(tokenName);
	// cout << "tokenName: " << tokenName << endl;
	if (it != _memberMap.end()) {
		PROC tokenProcessor = it->second;
		(this->*tokenProcessor)(tree, st, parent, childNum);
	}
}

// Create the root of the tree and first non-static symbol table, go to children
void TreeWalker::processPROG(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	boost::shared_ptr<SymbolTable> progSt(new SymbolTable(st));

	ASTNode root(progSt); // Maybe we need a different node for this?
	_outputTree->setRoot(&root);

	for (int i = 0; i < tree->getChildCount(tree); ++i) {
		walk(childByNum(tree, i), progSt, &root, i);
	}
}

// Create the param list, get the id, create node and walk to body
void TreeWalker::processPROCDEC(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	HeaderParamsAST params(st, childByNum(tree, 1));

	pANTLR3_BASE_TREE idTree = childByNum(tree, 0);
	string procName = createStringFromTree(idTree);

	ProcDecAST dec(st, procName, &params);
	parent->addChild(&dec, childNum);

	walk(childByNum(tree, 2), st, &dec, 2);
}

// We don't need a AST node here, but we do need a new symbol table as we have a new scope
void TreeWalker::processBODY(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	boost::shared_ptr<SymbolTable> scopeSt(new SymbolTable(st));

    for (int i = 0; i < tree->getChildCount(tree); ++i) {
        walk(childByNum(tree, i), scopeSt, parent, i);
    }
}

// Like PROCDEC but with a type
void TreeWalker::processFUNCDEC(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	HeaderParamsAST params(st, childByNum(tree, 1));

	pANTLR3_BASE_TREE idTree = (childByNum(tree, 0));
	pANTLR3_BASE_TREE typeTree = (childByNum(tree, 2));

	// Extract method from here
	string funcName = createStringFromTree(idTree);
	string funcType = createStringFromTree(typeTree);

	FuncDecAST dec(st, funcName, &params, funcType);

	parent->addChild(&dec, childNum);

	walk(childByNum(tree, 3), st, &dec, 3);
}

// Work out if it's an array or not, then process accordingly
void TreeWalker::processVARDEC(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	pANTLR3_BASE_TREE idTree = childByNum(tree, 0);
	string varName = createStringFromTree(idTree);

	pANTLR3_BASE_TREE varOptionsTree = childByNum(tree, 1);
	string typeName = createStringFromTree(childByNum(varOptionsTree, 0));
	pANTLR3_BASE_TREE exprTree = childByNum(varOptionsTree, 1);

	if (createStringFromTree(varOptionsTree) == "NEWVAR") {
		if (exprTree == NULL) {
			VariableDecAST dec(st, typeName, varName);
			parent->addChild(&dec, childNum);
		} else {
			ExprAST expr(st, exprTree);
			VariableDecAST dec(st, typeName, varName, &expr);
			dec.addChild(&expr, 0);
			parent->addChild(&dec, childNum);
		}
	} else {
		ExprAST expr(st, exprTree);
		ArrayDecAST dec(st, &expr, typeName);
		dec.addChild(&expr, 0);
		parent->addChild(&dec, childNum);
	}
}

void TreeWalker::processVARSTAT(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	string varId = createStringFromTree(childByNum(tree, 0));

	pANTLR3_BASE_TREE optionsTree = childByNum(tree, 1);
	string option = createStringFromTree(optionsTree);

	if (option == "ARRMEMBER") {
		ExprAST elem(st, childByNum(optionsTree, 0));
		ExprAST val(st, childByNum(optionsTree, 1));
		ArrayAssignAST assign(st, varId, &elem, &val);
		parent->addChild(&assign, childNum);
	} else if (option == "FUNC") {
		CallParamsAST params(st, childByNum(optionsTree, 0));
		// FuncAST func(st, varId, &params);
		// parent->addChild(&func, childNum);
	} else if (option == "ASSIGN") {
		ExprAST expr(st, childByNum(optionsTree, 0));
		VarAssignAST assign(st, varId, &expr);
		parent->addChild(&assign, childNum);
	}
}

void TreeWalker::processINC(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	IncAST inc(st, &expr);
	parent->addChild(&inc, childNum);
}

void TreeWalker::processDEC(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	DecAST dec(st, &expr);
	parent->addChild(&dec, childNum);
}

void TreeWalker::processPRINT(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	pANTLR3_BASE_TREE optionTree = childByNum(tree, 0);
	string option = createStringFromTree(optionTree); // Check this

	if (option == "ARRMEMBER") {
		string arrayID = createStringFromTree(childByNum(optionTree, 0));
		ExprAST expr(st, childByNum(optionTree, 1));
		PrintAST print(st, arrayID, &expr);
		parent->addChild(&print, childNum);
	} else if (option == "FUNC") {
		string funcID = createStringFromTree(childByNum(optionTree, 0));
		CallParamsAST params(st, childByNum(optionTree, 1));
		PrintAST print(st, funcID, &params);
		parent->addChild(&print, childNum);
	} else {
		ExprAST expr(st, optionTree);
		PrintAST print(st, &expr);
		parent->addChild(&print, childNum);
	}
}

void TreeWalker::processRETURN(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	ReturnAST ret(st, &expr);
	parent->addChild(&ret, childNum);
}

void TreeWalker::processSTDIN(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	StdinAST in(st, &expr);
	parent->addChild(&in, childNum);
}

void TreeWalker::processWHILE(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	WhileAST whilenode(st, &expr);
	parent->addChild(&whilenode, childNum);

	for (int i = 1; i < tree->getChildCount(tree); ++i) {
		walk(childByNum(tree, i), st, &whilenode, i - 1);
	}
}

void TreeWalker::processCHOICE(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	ChoiceAST choice(st, &expr);
	parent->addChild(&choice, childNum);

	for (int i = 1; i < tree->getChildCount(tree); ++i) {
		walk(childByNum(tree, i), st, &choice, i - 1);
	}
}

void TreeWalker::processIF(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {
	ExprAST expr(st, childByNum(tree, 0));
	IfAST ifnode(st, &expr);
	parent->addChild(&ifnode, childNum);

	for (int i = 1; i < tree->getChildCount(tree); ++i) {
		walk(childByNum(tree, i), st, &ifnode, i - 1);
	}
}

void TreeWalker::processNS(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, ASTNode* parent, int childNum) {}

// Code shortening methods:

pANTLR3_BASE_TREE TreeWalker::childByNum(pANTLR3_BASE_TREE tree, int num) {
	return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string TreeWalker::createStringFromTree(pANTLR3_BASE_TREE tree) {
	string res((const char *) tree->getText(tree)->to8(tree->getText(tree))->chars, 
			   tree->getText(tree)->len);
	return res;
}

// map intialisation
void TreeWalker::init() {
	_memberMap["PROG"] 	    = &TreeWalker::processPROG;
	_memberMap["PROCDEC"]   = &TreeWalker::processPROCDEC;
	_memberMap["BODY"]		= &TreeWalker::processBODY;
	_memberMap["FUNCDEC"]   = &TreeWalker::processFUNCDEC;
	_memberMap["VARDEC"]    = &TreeWalker::processVARDEC;
	_memberMap["VARSTAT"]   = &TreeWalker::processVARSTAT;
	_memberMap["INC"]       = &TreeWalker::processINC;
	_memberMap["DEC"]       = &TreeWalker::processDEC;
	_memberMap["PRINT"]     = &TreeWalker::processPRINT;
	_memberMap["RETURN"]    = &TreeWalker::processRETURN;
	_memberMap["STDIN"]     = &TreeWalker::processSTDIN;
	_memberMap["WHILE"]     = &TreeWalker::processWHILE;
	_memberMap["CHOICE"]    = &TreeWalker::processCHOICE;
	_memberMap["IF"]        = &TreeWalker::processIF;
	_memberMap["NS"]		= &TreeWalker::processNS;
}
