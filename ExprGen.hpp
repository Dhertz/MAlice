#ifndef EXPR_GEN_H
#define EXPR_GEN_H

#include <utility>
#include <string>
#include <list>
#include "AssemCom.hpp"
#include <boost/shared_ptr.hpp>
#include "ast/ExprAST.hpp"

using namespace std;

class ExprGen {
public:
	static pair< string, list<AssemCom> > generateExpression(pANTLR3_BASE_TREE expr, boost::shared_ptr<SymbolTable> st);
};

#endif
