#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include "AssemCom.hpp"
#include "AssemFunc.hpp"
#include <string>
#include <list>
#include "idents/Type.hpp"
#include "SymbolTable.hpp"
#include <boost/enable_shared_from_this.hpp>

// These are here to avoid circular includes
class ASTNode;
class IfBodyAST;
class ExprAST;
class CallParamsAST;
class HeaderParamsAST;

class ASTVisitor : public boost::enable_shared_from_this<ASTVisitor> {
	vector<boost::shared_ptr<AssemFunc> > _functions;
	list<AssemCom> _startDefs;
	list<AssemCom> _globalInlines;
	list<AssemCom> _endDefs;
	boost::shared_ptr<SymbolTable> _globalSt;
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
    
    vector<boost::shared_ptr<AssemFunc> > getFunctions();
    list<AssemCom>& getEndDefs();
    list<AssemCom> getStartDefs();
    list<AssemCom> getGlobalInlines();
};

#endif
