#ifndef AST_VISITOR
#define AST_VISITOR

#include "ast/ASTNode.hpp"
#include "ast/IfBodyAST.hpp"
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
	void visitINC(boost::shared_ptr<ExprAST> expr);
	void visitDEC(boost::shared_ptr<ExprAST> expr);
	void visitPRINT(boost::shared_ptr<ExprAST> expr);
	void visitRETURN();
	void visitSTDIN();
	void visitWHILE();
	void visitCHOICE(boost::shared_ptr<ExprAST> cond, 
					   boost::shared_ptr<IfBodyAST> trueBody, 
					   boost::shared_ptr<IfBodyAST> falseBody);
	void visitIF(boost::shared_ptr<ExprAST> cond,
				   boost::shared_ptr<IfBodyAST> trueBody, 
				   vector <boost::shared_ptr<ASTNode> > children);
	void visitSTDIN(boost::shared_ptr<ExprAST> expr);
	void visitWHILE(boost::shared_ptr<ExprAST> cond);
	void visitNS();
};

#endif