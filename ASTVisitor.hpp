#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include "AssemCom.hpp"
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
	list<AssemCom> _instrs;
	boost::shared_ptr<SymbolTable> _st;
	vector<string> _freeRegs;
public:
	ASTVisitor(boost::shared_ptr<SymbolTable> st);
	void visitProg(vector <boost::shared_ptr<ASTNode> > children);
	void visitProcDec(string name, boost::shared_ptr<HeaderParamsAST> params);
	void visitFuncDec(string name, string returnType, 
						boost::shared_ptr<HeaderParamsAST> params);
	void visitVarDec(string typeName, string varName);
	void visitInc(boost::shared_ptr<ExprAST> expr);
	void visitDec(boost::shared_ptr<ExprAST> expr);
	void visitPrint(boost::shared_ptr<ExprAST> expr);
	void visitReturn(boost::shared_ptr<ExprAST> expr);
	void visitStdin(boost::shared_ptr<ExprAST> expr);
	void visitWhile(boost::shared_ptr<ExprAST> cond, 
				      vector <boost::shared_ptr<ASTNode> > children);
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
