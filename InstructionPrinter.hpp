#ifndef INSTRUCTION_PRINTER
#define INSTRUCTION_PRINTER

#include "AssemCom.hpp"
#include <list>

using namespace std;

class InstructionPrinter {

public:
	static void printList(list<AssemCom> instrs);	

};

#endif