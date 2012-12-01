#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include "ast/ASTNode.hpp"
#include "ast/IfBodyAST.hpp"
#include "ast/HeaderParamsAST.hpp"
#include "ast/ExprAST.hpp"
#include "ast/CallParamsAST.hpp"
#include <string>

class ASTVisitor {
public:
	void visitProg();
	void visitProcDec(string name, boost::shared_ptr<HeaderParamsAST> params);
	void visitFuncDec(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params);
	void visitVarDec(string typeName, string varName);
	void visitInc(boost::shared_ptr<ExprAST> expr);
	void visitDec(boost::shared_ptr<ExprAST> expr);
	void visitPrint(boost::shared_ptr<ExprAST> expr);
	void visitReturn(boost::shared_ptr<ExprAST> expr);
	void visitStdin(boost::shared_ptr<ExprAST> expr);
	void visitWhile(boost::shared_ptr<ExprAST> cond);
	void visitChoice(boost::shared_ptr<ExprAST> cond, 
					   boost::shared_ptr<IfBodyAST> trueBody, 
					   boost::shared_ptr<IfBodyAST> falseBody);
	void visitIf(boost::shared_ptr<ExprAST> cond,
				   boost::shared_ptr<IfBodyAST> trueBody, 
				   vector <boost::shared_ptr<ASTNode> > children);
	void visitVarAss(string varName, boost::shared_ptr<ExprAST> expr);
	void visitFuncCall(string name, boost::shared_ptr<CallParamsAST> params);
	void visitArrayAssign(string name, boost::shared_ptr<ExprAST> index,
                   boost::shared_ptr<ExprAST> value);
    void visitArrayDec(string name, boost::shared_ptr<ExprAST> length,
                         boost::shared_ptr<Type> type);
};

#endif
