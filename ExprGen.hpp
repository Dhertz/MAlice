#ifndef EXPR_GEN_H
#define EXPR_GEN_H

#include <boost/tuple/tuple.hpp>
#include <string>
#include <list>
#include "AssemCom.hpp"
#include <boost/shared_ptr.hpp>
#include "ast/ExprAST.hpp"

using namespace std;

class ExprGen {
public:
	static boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > generateExpression(pANTLR3_BASE_TREE root, boost::shared_ptr<SymbolTable> st, vector<string> freeRegs, boost::shared_ptr<AssemFunc> func);
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
