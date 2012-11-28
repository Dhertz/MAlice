#ifndef AST_VISITOR
#define AST_VISITOR

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
	void visitCHOICE();
	void visitIF();
	void visitNS();
};

#endif