#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include "ast/ASTNode.hpp"
#include "ast/IfBodyAST.hpp"
#include "ast/HeaderParamsAST.hpp"
#include "ast/ExprAST.hpp"
#include <string>

class ASTVisitor {
public:
	void visitProcDec(string name, boost::shared_ptr<HeaderParamsAST> params);
	void visitFuncDec(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params);
	void visitVarDec(string typeName, string varName);
	void visitInc(boost::shared_ptr<ExprAST> expr);
	void visitDec(boost::shared_ptr<ExprAST> expr);
	void visitPrint(boost::shared_ptr<ExprAST> expr);
	void visitReturn(boost::shared_ptr<ExprAST> expr);
	void visitStdin(boost::shared_ptr<ExprAST> expr);
	void visitWhile(boost::shared_ptr<ExprAST> cond);
	void visitCHOICE(boost::shared_ptr<ExprAST> cond, 
					   boost::shared_ptr<IfBodyAST> trueBody, 
					   boost::shared_ptr<IfBodyAST> falseBody);
	void visitIF(boost::shared_ptr<ExprAST> cond,
				   boost::shared_ptr<IfBodyAST> trueBody, 
				   vector <boost::shared_ptr<ASTNode> > children);
};

#endif
