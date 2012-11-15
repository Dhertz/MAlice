#ifndef PRINT_AST
#define PRINT_AST

class PrintAST : public ASTNode {

public:
	PrintAST(SymbolTable* st, string arrayName, ExprAST* element);
	PrintAST(SymbolTable* st, string funcName, CallParamsAST* params);
	PrintAST(SymbolTable* st, ExprAST* expr);
};

#endif