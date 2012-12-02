#include "ast/ASTNode.hpp"
#include "ast/IfBodyAST.hpp"
#include "ast/HeaderParamsAST.hpp"
#include "ast/ExprAST.hpp"
#include "ast/CallParamsAST.hpp"
#include "ASTVisitor.hpp"
#include "idents/Variable.hpp"
#include "Label.hpp"
#include "ExprGen.hpp"
#include <boost/tuple/tuple.hpp>

void ASTVisitor::visitProg(vector <boost::shared_ptr<ASTNode> > children, 
							 boost::shared_ptr<SymbolTable> st) {
	// I imagine some bollocks will go here...

	vector<boost::shared_ptr<ASTNode> >::iterator i;
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this());
	}

	// And here...
}

void ASTVisitor::visitProcDec(string name, 
								boost::shared_ptr<HeaderParamsAST> params, 
								boost::shared_ptr<SymbolTable> st) {
	vector<string> alignArg;
	alignArg.push_back("2");
	_instrs.push_back(AssemCom(".align", 1, alignArg));							// .align 2

	_instrs.push_back(AssemCom(name + ":", 0, std::vector<string>()));			// name:
}

void ASTVisitor::visitFuncDec(string name, string returnType, 
								boost::shared_ptr<HeaderParamsAST> params, 
								boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitVarDec(string typeName, string varName, 
							   boost::shared_ptr<SymbolTable> st) {
	boost::shared_ptr<Identifier> varIdent = st->lookupCurrLevelOnly(varName);
	if (varIdent) {
		boost::shared_ptr<Variable> var = 
		  boost::shared_polymorphic_downcast<Variable>(varIdent);
		boost::shared_ptr<Type> varType = var->getTypeName();
		// instruction to allocate memory for global of type
	}
}

void ASTVisitor::visitInc(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();

	vector<string> incArgs;
	incArgs.push_back(resultReg);
	incArgs.push_back(resultReg);
	incArgs.push_back("#1");

	_instrs.push_back(AssemCom("add", 3, incArgs));								// add resultReg resultReg #1
}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr,
							boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();

	vector<string> decArgs;
	decArgs.push_back(resultReg);
	decArgs.push_back(resultReg);
	decArgs.push_back("#1");

	// May need to be "subs"
	_instrs.push_back(AssemCom("sub", 3, decArgs));								// sub resultReg resultReg #1
}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st) {
	// I don't think I care about the expression, that can be handled in the 
	// function call

	vector<string> retArg;
	retArg.push_back("lr");

	_instrs.push_back(AssemCom("bx", 1, retArg));								// bx lr
}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitWhile(boost::shared_ptr<ExprAST> cond, 
			   			   	  vector <boost::shared_ptr<ASTNode> > children, 
							  boost::shared_ptr<SymbolTable> st) {
	Label loopLabel;

	_instrs.push_back(
	   AssemCom(loopLabel.getLabel() + ":", 0, std::vector<string>()));			// loop:

	vector<boost::shared_ptr<ASTNode> >::iterator i;							// loop body
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this());
	}

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");

	_instrs.push_back(AssemCom("cmp", 2, cmpArgs));								// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(loopLabel.getLabel());

	_instrs.push_back(AssemCom("bne", 1, bneArgs));								// bne loop
}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody, 
							   boost::shared_ptr<SymbolTable> st) {
	Label elseLabel;

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	std::vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");

	_instrs.push_back(AssemCom("cmp", 2, cmpArgs));								// cmp resultReg #0

	std::vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());

	_instrs.push_back(AssemCom("bne", 1, bneArgs));								// bne else

	trueBody->accept(shared_from_this());										// if body

	Label endLabel;
	vector<string> bArgs;
	bArgs.push_back(endLabel.getLabel());
	_instrs.push_back(AssemCom("b", 1, bArgs));									// b end

	_instrs.push_back(
	   AssemCom(elseLabel.getLabel() + ":", 0, std::vector<string>()));			// else:

	falseBody->accept(shared_from_this());										// else body

	_instrs.push_back(
	   AssemCom(endLabel.getLabel() + ":", 0, std::vector<string>()));			// end:
}

void ASTVisitor::visitIf(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children, 
						   boost::shared_ptr<SymbolTable> st) {
	Label elseLabel;

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");

	_instrs.push_back(AssemCom("cmp", 2, cmpArgs));								// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());

	_instrs.push_back(AssemCom("bne", 1, bneArgs));								// bne else

	trueBody->accept(shared_from_this());										// if body

	_instrs.push_back(
	   AssemCom(elseLabel.getLabel() + ":", 0, std::vector<string>()));			// else:

	// Loop through until possible else node
	vector<boost::shared_ptr<ASTNode> >::iterator i;
	for (i = children.begin(); i != children.end() - 1; ++i) {
		(*i)->accept(shared_from_this());
	}

	Label endLabel;
	vector<string> bArgs;
	bArgs.push_back(endLabel.getLabel());
	_instrs.push_back(AssemCom("b", 1, bArgs));									// b end

	// Do the else node manually (the iterator is already in the right place)
	// Not sure if doing the else node seperatly is necessary?
	(*i)->accept(shared_from_this());											// else body

	_instrs.push_back(
	   AssemCom(endLabel.getLabel() + ":", 0, std::vector<string>()));			// end:
}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitFuncCall(string name,
						    	 boost::shared_ptr<CallParamsAST> params, 
							     boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitArrayAssign(string name,
                  					boost::shared_ptr<ExprAST> index,
                  					boost::shared_ptr<ExprAST> value, 
							        boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length,
                                 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st) {}

list<AssemCom> ASTVisitor::getInstrs() {
	return _instrs;
}
