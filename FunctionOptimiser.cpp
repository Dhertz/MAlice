#include "FunctionOptimiser.hpp"
#include <list>
#include <algorithm>
#include <boost/lexical_cast.hpp>

/*
	FunctionOptimiser::optimise:
	- Remove empty conditions (This happens in ASTVisitor now)
	- Track function calls
	- Remove unecessary mov instructions
	- Remove unecessary mov sequences
*/
std::map<string, int> 
FunctionOptimiser::optimise(boost::shared_ptr<AssemFunc>& func, 
							  std::map<string, int> calls,
							  std::map<int, int> duplicateLabels) {
	list<AssemCom>& comms = func->getComms();
	list<AssemCom>::iterator it;
	for (it = comms.begin(); it != comms.end(); ++it) {
		string name = it->getName();
		vector<string>& args = it->getArgs();

		if (name == "mov") {

			if (args[0] == args[1] && args.size() == 2) {
				cout << "hi" << endl;
				// mov is uncessary, remove it
				it = comms.erase(it);
				continue;
			}

			AssemCom nextComm = *(it++);
			string next = nextComm.getName();
			--it;

			if (nextComm.getName() == "mov") {
				if (args[0] == nextComm.getArgs()[1] && 
					args[1] == nextComm.getArgs()[0] && args.size() == 2 && 
					nextComm.getArgs().size() == 2) {
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
			} else {;
				calls[calledFunction]++;
			}
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

	return calls;
}
