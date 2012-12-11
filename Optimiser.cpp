#include "Optimiser.hpp"
#include "AssemFunc.hpp"
#include "FunctionOptimiser.hpp"
#include "InstructionPrinter.hpp"
#include <boost/unordered_map.hpp>

Optimiser::Optimiser(boost::shared_ptr<ASTVisitor> v, string filename) {
	_visitor = v;
	_filename = filename;
	_program = _visitor->getStartDefs();
	processFunctions();
}

void Optimiser::processFunctions() {
	vector<boost::shared_ptr<AssemFunc> > functions = _visitor->getFunctions();
	vector<boost::shared_ptr<AssemFunc> >::iterator i;

	list<AssemCom> end = removeDuplicateLabels();

	map<string, int> calls;

	for (i = functions.begin(); i != functions.end(); ++i) {
		if ((*i)->getName() == "main") {
			(*i)->addListFront(_visitor->getGlobalInlines());
		}
		(*i)->finalise();
		calls[(*i)->getName()] = 0;
		FunctionOptimiser::optimise((*i), calls, _duplicateLabels);
	}

	// Remove unreachable functions from code
	i = functions.begin();
	for (; i != functions.end(); ++i) {
		if ((*i)->getName() != "main") {
			if (calls[(*i)->getName()] == 0) {
				// This function is not called, so we don't add it to _program
				continue;
			}
		}
		_program.splice(_program.end(), (*i)->getComms());
	}

	_program.splice(_program.end(), end);

	InstructionPrinter::printList(_program, _filename.append(".s"));
}

list<AssemCom>& Optimiser::removeDuplicateLabels() {
	list<AssemCom>::iterator it = _visitor->getEndDefs().begin();
	map<string, AssemCom> labels;
	for (; it != _visitor->getEndDefs().end(); ++it) {
		if (it->getName()[1] == 'L') {
			// We have a label
			++it;
			bool commSeenBefore = false;
			int previousLabel;
			map<string, AssemCom>::iterator lblIt = labels.begin();
			for (; lblIt != labels.end(); ++lblIt) {
				if (lblIt->second == *it) {
					// This command has already been seen
					commSeenBefore = true;
					previousLabel = atoi(lblIt->first.substr(2).c_str());
					break;
				}
			}

			if (commSeenBefore) {
				--it;
				int length = it->getName().size() - 3;
				// mark this label as a duplciate for later and remove the
				// duplicate instructions
				_duplicateLabels.insert(
					pair<int, int>(atoi(it->getName().substr(2).c_str()), previousLabel));
				it = _visitor->getEndDefs().erase(it);
				it = _visitor->getEndDefs().erase(it);
				--it;
			} else {
				AssemCom c = *it;
				--it;
				labels.insert(pair<string, AssemCom>(it->getName(), c));
			}
		}
	}

	return _visitor->getEndDefs();
}
