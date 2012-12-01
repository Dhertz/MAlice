#include "ASTVisitor.hpp"

void ASTVisitor::visitPROG() {}

void ASTVisitor::visitPROCDEC() {}

void ASTVisitor::visitBODY() {}

void ASTVisitor::visitFUNCDEC() {}

void ASTVisitor::visitVARDEC() {}

void ASTVisitor::visitVARSTAT() {}

void ASTVisitor::visitINC() {}

void ASTVisitor::visitDEC() {}

void ASTVisitor::visitPRINT() {}

void ASTVisitor::visitRETURN() {}

void ASTVisitor::visitSTDIN() {}

void ASTVisitor::visitWHILE() {}

void ASTVisitor::visitCHOICE(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody) {}

void ASTVisitor::visitIF(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children) {}

void ASTVisitor::visitNS() {}
