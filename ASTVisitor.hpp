#ifndef AST_VISITOR
#define AST_VISITOR

#include "ast/HeaderParamsAST.hpp"
#include "ast/ExprAST.hpp"
#include <string>
#include <boost/shared_ptr.hpp>

class ASTVisitor {

public:
	void visitPROG();
	void visitPROCDEC(string name, boost::shared_ptr<HeaderParamsAST> params);
	void visitBODY();
	void visitFUNCDEC(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params);
	void visitVARDEC(string typeName, string varName);
	void visitVARSTAT();
	void visitINC();
	void visitDEC();
	void visitPRINT();
	void visitRETURN();
	void visitSTDIN();
	void visitWHILE();
	void visitCHOICE();
	void visitIF(boost::shared_ptr<ExprAST> cond);
	void visitNS();
};

#endif