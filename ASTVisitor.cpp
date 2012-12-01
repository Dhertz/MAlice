#include "ASTVisitor.hpp"

void ASTVisitor::visitPROG() {}

void ASTVisitor::visitPROCDEC(string name, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitBODY() {}

void ASTVisitor::visitFUNCDEC(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitVARDEC(string typeName, string varName) {}

void ASTVisitor::visitVARSTAT() {}

void ASTVisitor::visitINC(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitDEC(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitPRINT(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitRETURN() {}

void ASTVisitor::visitSTDIN(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitWHILE(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitCHOICE(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody) {}

void ASTVisitor::visitIF(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children) {}

void ASTVisitor::visitNS() {}
