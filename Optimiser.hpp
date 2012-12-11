#ifndef OPTIMISER
#define OPTIMISER

#include "ASTVisitor.hpp"
#include <boost/shared_ptr.hpp>
#include <map>
#include <list>
#include "AssemCom.hpp"

class Optimiser {
	boost::shared_ptr<ASTVisitor> _visitor;
	string _filename;
	map<int, int> _duplicateLabels;
	list<AssemCom> _program;

public:
	Optimiser(boost::shared_ptr<ASTVisitor> v, string filename);
	void processFunctions();
	list<AssemCom>& removeDuplicateLabels();
};

#endif