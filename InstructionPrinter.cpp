#include "InstructionPrinter.hpp"
#include <iostream>

void InstructionPrinter::printList(list<AssemCom> l) {
	list<AssemCom>::const_iterator it;

	cout << l.size() << endl;

	for (it = l.begin(); it != l.end(); ++it) {
		AssemCom comm = *it;

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
}
