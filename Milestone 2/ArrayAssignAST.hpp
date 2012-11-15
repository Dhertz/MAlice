#ifndef ARRAY_ASSIGN_AST
#define ARRAY_ASSIGN_AST

class ArrayAssignAST : public ASTNode {

public:
	ArrayAssignAST(SymbolTable* st, string name, ExprAST* element, ExprAST* value);
};

#endif