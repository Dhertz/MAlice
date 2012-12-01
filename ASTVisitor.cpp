#include "ASTVisitor.hpp"
#include "idents/Variable.hpp"

ASTVisitor::ASTVisitor(boost::shared_ptr<SymbolTable> st) {
	_st = st;
}

void ASTVisitor::visitProg() {}

void ASTVisitor::visitProcDec(string name, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitFuncDec(string name, string returnType, boost::shared_ptr<HeaderParamsAST> params) {}

void ASTVisitor::visitVarDec(string typeName, string varName) {
	boost::shared_ptr<Identifier> varIdent = _st->lookupCurrLevelOnly(varName);
	if (varIdent) {
		boost::shared_ptr<Variable> var = boost::shared_polymorphic_downcast<Variable>(varIdent);
		boost::shared_ptr<Type> varType = var->getTypeName();
		// instruction to allocate memory for global of type
	}
}

void ASTVisitor::visitInc(boost::shared_ptr<ExprAST> expr) {
	
}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitWhile(boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody) {}

void ASTVisitor::visitIf(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children) {}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr) {}

void ASTVisitor::visitFuncCall(string name,
						    	boost::shared_ptr<CallParamsAST> params) {}

void ASTVisitor::visitArrayAssign(string name,
                  boost::shared_ptr<ExprAST> index,
                  boost::shared_ptr<ExprAST> value) {}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length,
                                 boost::shared_ptr<Type> type) {}
