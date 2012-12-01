#include "ASTVisitor.hpp"

void ASTVisitor::visitPROG() {}

void ASTVisitor::visitPROCDEC(string name, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitBODY() {}

void ASTVisitor::visitFUNCDEC(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitVARDEC(string typeName, string varName) {}

void ASTVisitor::visitVARSTAT() {}

void ASTVisitor::visitINC() {}

void ASTVisitor::visitDEC() {}

void ASTVisitor::visitPRINT() {}

void ASTVisitor::visitRETURN() {}

void ASTVisitor::visitSTDIN() {}

void ASTVisitor::visitWHILE() {}

void ASTVisitor::visitCHOICE() {}

void ASTVisitor::visitIF(boost::shared_ptr<ExprAST> cond) {}

void ASTVisitor::visitNS() {}
