#include "FunctionOptimiser.hpp"
#include <list>
#include <algorithm>
#include <boost/lexical_cast.hpp>

/*
	FunctionOptimiser::optimise:
	- Remove empty conditions
	- Track function calls
	- Remove unecessary mov instructions
	- Remove unecessary mov sequences
*/
void FunctionOptimiser::optimise(boost::shared_ptr<AssemFunc>& func, 
								   std::map<string, int>& calls,
								   std::map<int, int> duplicateLabels) {
	list<AssemCom>& comms = func->getComms();
	list<AssemCom>::iterator it;
	for (it = comms.begin(); it != comms.end(); ++it) {
		string name = it->getName();
		vector<string>& args = it->getArgs();

		if (name == "mov") {

			if (args[0] == args[1]) {
				// mov is uncessary, remove it
				it = comms.erase(it);
				continue;
			}

			AssemCom nextComm = *(it++);
			string next = nextComm.getName();

			if (nextComm.getName() == "mov") {
				if (args[0] == nextComm.getArgs()[1] && 
					args[1] == nextComm.getArgs()[0]) {
					// mov sequence is unecessary
					// remove both commands
					it = comms.erase(it);
					it = comms.erase(it);
					continue;
				}
			}
		} else if (name == "bl") {
			// track function call
			string calledFunction = args[0];
			if (calls.find(calledFunction) == calls.end()) {
				calls[calledFunction] = 1;
			} else {
				calls[calledFunction]++;
			}
		} else if (name == "beq") {
			string labelName = args[0];
			AssemCom nextComm = *(++it);
			if (labelName + ":" == nextComm.getName()) {
				//Unecessary condition, let's remove it

				// First trace back to start of condition check
				list<AssemCom>::iterator end = ++it;
				vector<string> regs;
				while (it->getName() != "eor") {
					it--;
					AssemCom trackBack = *(it);
					if (trackBack.getName() == "cmp") {
						// We also want to remove any instructions that alter 
						// registers used in comparions, track those registers 
						// as we track back
						if (trackBack.getArgs()[0][0] == 'r') {
							regs.push_back(trackBack.getArgs()[0]);
						}
						if (trackBack.getArgs()[1][0] == 'r') {
							regs.push_back(trackBack.getArgs()[1]);
						}
					}
				}
				// Then remove all commands between eor and beq
				it = comms.erase(it, end);

				if ((--it)->getName() == "mov") {
					string reg = it->getArgs()[0];
					if (find(regs.begin(), regs.end(), reg) != regs.end()) {
						// remove the mov instruction if it was used in a cmp
						it = comms.erase(it);
					}
				}
			}
			it--;
		} else {
			// Check that all labels are correct after removing duplicates
			vector<string>::iterator argIt = args.begin();
			for (; argIt != args.end(); ++argIt) {
				if ((*argIt).find("L") != string::npos) {
					int numPos;
					if ((*argIt)[0] == '=') {
						numPos = 3;
					} else {
						numPos = 2;
					}

					string numStr = (*argIt).substr(numPos);
					int num = atoi(numStr.c_str());
					if (duplicateLabels.find(num) != duplicateLabels.end()) {
						// The label has been replaced - replace it here as well
						(*argIt).erase(numPos);
						int newNum = duplicateLabels.find(num)->second;
						(*argIt).append(boost::lexical_cast<string>(newNum));
					}
				}
			}
		}
	}
}
