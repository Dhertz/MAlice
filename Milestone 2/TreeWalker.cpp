#include "TreeWalker.hpp"

// Check the current node, do required stuff based on its type
void TreeWalker::walk(pANTLR3_BASE_TREE tree) {
	string tokenName((const char *) tree->getText(tree)->chars, 
					 (size_t) tree->getText(tree)->size);
	void (TreeWalker::*tokenProcessor)() = memberMap[tokenName];
	(this->*tokenProcessor)();
}

void TreeWalker::createASTNode() {

}

void TreeWalker::init() {
	memberMap["PROG"] 	   = &TreeWalker::processPROG;
	memberMap["PROCDEC"]   = &TreeWalker::processPROCDEC;
	memberMap["FUNCDEC"]   = &TreeWalker::processFUNCDEC;
	memberMap["HPL"] 	   = &TreeWalker::processHPL;
	memberMap["VARDEC"]    = &TreeWalker::processVARDEC;
	memberMap["NEWVAR"]    = &TreeWalker::processNEWVAR;
	memberMap["NEWARR"]    = &TreeWalker::processNEWARR;
	memberMap["VARSTAT"]   = &TreeWalker::processVARSTAT;
	memberMap["ARRMEMBER"] = &TreeWalker::processARRMEMBER;
	memberMap["FUNC"] 	   = &TreeWalker::processFUNC;
	memberMap["CPL"] 	   = &TreeWalker::processCPL;
	memberMap["ASSIGN"]    = &TreeWalker::processASSIGN;
	memberMap["INC"]       = &TreeWalker::processINC;
	memberMap["DEC"]       = &TreeWalker::processDEC;
	memberMap["PRINT"]     = &TreeWalker::processPRINT;
	memberMap["RETURN"]    = &TreeWalker::processRETURN;
	memberMap["STDIN"]     = &TreeWalker::processSTDIN;
	memberMap["WHILE"]     = &TreeWalker::processWHILE;
	memberMap["CHOICE"]    = &TreeWalker::processCHOICE;
	memberMap["IF"]        = &TreeWalker::processIF;
	memberMap["COND"]      = &TreeWalker::processCOND;
	memberMap["EXPR"]      = &TreeWalker::processEXPR;
}