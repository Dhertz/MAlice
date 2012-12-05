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
	static boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > generateExpression(pANTLR3_BASE_TREE root, boost::shared_ptr<SymbolTable> st, vector<string> freeRegs);
};

#endif
