#include "InstructionPrinter.hpp"
//#include <fstream>
#include <iostream>

// NB: change cout to file and uncomment things to write to "filename.s" - Owen

void InstructionPrinter::printList(list<AssemCom> l, string filename) {
	list<AssemCom>::const_iterator it;

	//ofstream file(filename.c_str());

	//if (file.is_open()) {

		cout << endl;

		for (it = l.begin(); it != l.end(); ++it) {
			AssemCom comm = *it;

			if (comm.getArity() > 0) {
				cout << "\t";
			} else {
				cout << "\n";
			}
			cout << comm.getName() << "\t";

			vector<string> args = comm.getArgs();

			for (int i = 0; i < comm.getArity() - 1; ++i) {
				cout << args[i] << ", ";
			}

			if (comm.getArity() > 0) {
				cout << args[comm.getArity() - 1];
			}

			cout << endl;
		}
	//}
}
