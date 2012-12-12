#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include "AssemCom.hpp"
#include "AssemFunc.hpp"
#include <string>
#include <list>
#include "idents/Type.hpp"
#include "SymbolTable.hpp"
#include <boost/enable_shared_from_this.hpp>

// These class are forward declared to avoid circular header file includes
class ASTNode;
class IfBodyAST;
class ExprAST;
class CallParamsAST;
class HeaderParamsAST;

/*
	ASTVisitor
	----------
	This class uses the visitor pattern to visit any node it comes across in 
	an AST. For each node, it generates the required ARM Assembler code, 
	calling static methods from ExprGen when needed.
	It builds a list of functions, top and bottom definitions which can be
	passed to Optimiser and InstructionPrinter to produce output files.

	NB: Some visitor methods are overloaded, this is the case when the node can 
	occur inside and outside of a function definition
*/
class ASTVisitor : public boost::enable_shared_from_this<ASTVisitor> {
	vector<boost::shared_ptr<AssemFunc> > _functions;
	list<AssemCom> _startDefs;
	list<AssemCom> _globalInlines;
	list<AssemCom> _endDefs;
	boost::shared_ptr<SymbolTable> _globalSt;
	bool _init_io;
	void initFreeRegs();
public:
	ASTVisitor(boost::shared_ptr<SymbolTable> st);

	void visitProg(vector <boost::shared_ptr<ASTNode> > children);

	void visitProcDec(string name, boost::shared_ptr<HeaderParamsAST> params, 
						vector <boost::shared_ptr<ASTNode> > children,
						boost::shared_ptr<SymbolTable> st);

	void visitFuncDec(string name, string returnType, 
						boost::shared_ptr<HeaderParamsAST> params,  
						vector <boost::shared_ptr<ASTNode> > children,
						boost::shared_ptr<SymbolTable> st);

	void visitVarDec(string typeName, string varName, 
					   boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<AssemFunc> func);

	void visitInc(boost::shared_ptr<ExprAST> expr, 
					boost::shared_ptr<SymbolTable> st,
					boost::shared_ptr<AssemFunc> func);

	void visitDec(boost::shared_ptr<ExprAST> expr, 
					boost::shared_ptr<SymbolTable> st,
					boost::shared_ptr<AssemFunc> func);

	void visitPrint(boost::shared_ptr<ExprAST> expr, 
					  boost::shared_ptr<SymbolTable> st,
					  boost::shared_ptr<AssemFunc> func);

	void visitReturn(boost::shared_ptr<ExprAST> expr, 
					   boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<AssemFunc> func);

	void visitStdin(boost::shared_ptr<ExprAST> expr, 
					  boost::shared_ptr<SymbolTable> st,
					  boost::shared_ptr<AssemFunc> func);

	void visitWhile(boost::shared_ptr<ExprAST> cond, 
				      vector <boost::shared_ptr<ASTNode> > children, 
				      boost::shared_ptr<SymbolTable> st,
				      boost::shared_ptr<AssemFunc> func);

	void visitChoice(boost::shared_ptr<ExprAST> cond, 
					   boost::shared_ptr<IfBodyAST> trueBody, 
					   boost::shared_ptr<IfBodyAST> falseBody, 
					   boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<AssemFunc> func);

	void visitIf(boost::shared_ptr<ExprAST> cond,
				   boost::shared_ptr<IfBodyAST> trueBody, 
				   vector <boost::shared_ptr<ASTNode> > children, 
				   boost::shared_ptr<SymbolTable> st,
				   boost::shared_ptr<AssemFunc> func);

	void visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
					  boost::shared_ptr<SymbolTable> st,
					  boost::shared_ptr<AssemFunc> func);

	void visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
					  boost::shared_ptr<SymbolTable> st);

	void visitFuncCall(string name, boost::shared_ptr<CallParamsAST> params, 
						 boost::shared_ptr<SymbolTable> st,
						 boost::shared_ptr<AssemFunc> func);

	void visitArrayAssign(string name, boost::shared_ptr<ExprAST> index,
                  		 	boost::shared_ptr<ExprAST> value, 
                  		 	boost::shared_ptr<SymbolTable> st,
                  		 	boost::shared_ptr<AssemFunc> func);

    void visitArrayDec(string name, boost::shared_ptr<ExprAST> length,
                         boost::shared_ptr<Type> type, 
                         boost::shared_ptr<SymbolTable> st,
                         boost::shared_ptr<AssemFunc> func);
    
    void visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
						 boost::shared_ptr<Type> type, 
					     boost::shared_ptr<SymbolTable> st);
    void visitMakeIn(boost::shared_ptr<ExprAST> expr, 
					   boost::shared_ptr<SymbolTable> st,
				  	   boost::shared_ptr<AssemFunc> func);
    void visitMakeOut(boost::shared_ptr<ExprAST> expr, 
					    boost::shared_ptr<SymbolTable> st,
					    boost::shared_ptr<AssemFunc> func);
    void visitPause(boost::shared_ptr<ExprAST> expr, 
					  boost::shared_ptr<SymbolTable> st,
					  boost::shared_ptr<AssemFunc> func);
    void visitReadIn(boost::shared_ptr<ExprAST> expr, 
					   boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<AssemFunc> func);
    void visitSetHigh(boost::shared_ptr<ExprAST> expr, 
					    boost::shared_ptr<SymbolTable> st,
					    boost::shared_ptr<AssemFunc> func);
    void visitSetLow(boost::shared_ptr<ExprAST> expr, 
					   boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<AssemFunc> func);
    
    void addLabel(boost::shared_ptr<AssemFunc> f, string comm);
    void addCommand(boost::shared_ptr<AssemFunc> f, string comm, string arg0);
    void addCommand(boost::shared_ptr<AssemFunc> f, string comm, string arg0, 
    				  string arg1);
    void addCommand(boost::shared_ptr<AssemFunc> f, string comm, string arg0, 
    				  string arg1, string arg2);

    vector<boost::shared_ptr<AssemFunc> > getFunctions();
    list<AssemCom>& getEndDefs();
    list<AssemCom> getStartDefs();
    list<AssemCom> getGlobalInlines();
};

#endif
