#ifndef AST_VISITOR
#define AST_VISITOR

#include "ast/ASTNode.hpp"
#include "ast/IfBodyAST.hpp"
#include "ast/ExprAST.hpp"

class ASTVisitor {

public:
	void visitPROG();
	void visitPROCDEC();
	void visitBODY();
	void visitFUNCDEC();
	void visitVARDEC();
	void visitVARSTAT();
	void visitINC();
	void visitDEC();
	void visitPRINT();
	void visitRETURN();
	void visitSTDIN();
	void visitWHILE();
	void visitCHOICE(boost::shared_ptr<ExprAST> cond, 
					   boost::shared_ptr<IfBodyAST> trueBody, 
					   boost::shared_ptr<IfBodyAST> falseBody);
	void visitIF(boost::shared_ptr<ExprAST> cond,
				   boost::shared_ptr<IfBodyAST> trueBody, 
				   vector <boost::shared_ptr<ASTNode> > children);
	void visitNS();
};

#endif