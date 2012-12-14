#include "InstructionPrinter.hpp"
#include <fstream>
#include <iostream>

/*
	InstructionPrinter::printList
	-----------------------------
	static method which takes a list of commands and an output file name
	and turns each AssemCom into an actual valid ARM Assembler command.
	Attempts to tab the assembly code out nicely as well.
*/
void InstructionPrinter::printList(list<AssemCom> l, string filename) {
	list<AssemCom>::const_iterator it;

	ofstream file(filename.c_str());

	if (file.is_open()) {

		file << endl;

		for (it = l.begin(); it != l.end(); ++it) {
			AssemCom comm = *it;

			if (comm.getArity() > 0) {
				file << "\t";
			}

			if (comm.getName().size() < 4) {
				file << comm.getName() << "\t" << "\t";
			} else {
				file << comm.getName() << "\t";
			}

			vector<string>& args = comm.getArgs();

			for (int i = 0; i < comm.getArity() - 1; ++i) {
				file << args[i] << ", ";
			}

			if (comm.getArity() > 0) {
				file << args[comm.getArity() - 1];
			}

			file << endl;
		}
	}
}
