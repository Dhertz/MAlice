#include "ASTVisitor.hpp"

void ASTVisitor::visitProcDec(string name, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitFuncDec(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitVarDec(string typeName, string varName) {}

void ASTVisitor::visitInc(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitWhile(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitCHOICE(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody) {}

void ASTVisitor::visitIF(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children) {}
