#ifndef	TREE_WALKER_H
#define TREE_WALKER_H 

#include <antlr3.h>
#include <map>
#include <string>

using namespace std;

class TreeWalker {
	void createASTNode();
	void init();
	map<string, void (TreeWalker::*)()> memberMap;
	void processPROG();
	void processPROCDEC();
	void processFUNCDEC();
	void processHPL();
	void processVARDEC();
	void processNEWVAR();
	void processNEWARR();
	void processVARSTAT();
	void processARRMEMBER();
	void processFUNC();
	void processCPL();
	void processASSIGN();
	void processINC();
	void processDEC();
	void processPRINT();
	void processRETURN();
	void processSTDIN();
	void processWHILE();
	void processCHOICE();
	void processIF();
	void processCOND();
	void processEXPR();

public:
	void walk(pANTLR3_BASE_TREE tree);
};

#endif