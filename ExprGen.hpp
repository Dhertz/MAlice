#ifndef EXPR_GEN_H
#define EXPR_GEN_H

#include <boost/tuple/tuple.hpp>
#include <string>
#include <list>
#include <map>
#include "AssemCom.hpp"
#include <boost/shared_ptr.hpp>
#include "ast/ExprAST.hpp"

using namespace std;

/*
	ExprGen
	-------
	Some static methods used for tasks to do with expressions: generating 
	instructions, evaluating constant expressions at compile time and weighting
	expressions in order to improv register allocation
*/
class ExprGen {
public:
	static map<int, string> _constRegs;

	static 
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > 
	generateExpression(pANTLR3_BASE_TREE root, 
						 boost::shared_ptr<SymbolTable> st, 
						 vector<string> freeRegs, 
						 boost::shared_ptr<AssemFunc> func);

	static int evaluateExpression(pANTLR3_BASE_TREE root, 
									boost::shared_ptr<SymbolTable> st);

	static int calculateWeight(pANTLR3_BASE_TREE tree, vector<string> freeRegs,
								 boost::shared_ptr<SymbolTable> st);

	static void addCommand(list<AssemCom>& instrs, string name, string arg0);

	static void addCommand(list<AssemCom>& instrs, string name, string arg0, 
							 string arg1);

	static void addCommand(list<AssemCom>& instrs, string name, string arg0, 
							 string arg1, string arg2);
};

#endif
