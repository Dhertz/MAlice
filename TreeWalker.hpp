#ifndef	TREE_WALKER_H
#define TREE_WALKER_H 

#include <antlr3.h>
#include <map>
#include "ast/AST.hpp"

using namespace std;

class TreeWalker {
	typedef void (TreeWalker::*PROC)(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode>, int);
	map<string, PROC> _memberMap;
	boost::shared_ptr<SymbolTable> _topSt;
	pANTLR3_BASE_TREE _inputTree;
	boost::shared_ptr<AST> _outputTree;
	void init();
	void processPROG(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processPROCDEC(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processBODY(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processFUNCDEC(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processVARDEC(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processVARSTAT(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processINC(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processDEC(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processPRINT(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processRETURN(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processSTDIN(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processWHILE(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processCHOICE(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processIF(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processCOND(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	void processNS(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	bool findReturn(pANTLR3_BASE_TREE tree);
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	string createStringFromTree(pANTLR3_BASE_TREE tree);
	int getLine(pANTLR3_BASE_TREE tree);
	void checkHatta();

public:
	void walk(pANTLR3_BASE_TREE, boost::shared_ptr<SymbolTable>, boost::shared_ptr<ASTNode> parent, int childNum);
	TreeWalker(boost::shared_ptr<SymbolTable> topSt, pANTLR3_BASE_TREE inputTree, boost::shared_ptr<AST> outputTree);
};

#endif