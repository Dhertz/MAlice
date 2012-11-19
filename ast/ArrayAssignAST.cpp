#include "ArrayAssignAST.hpp"

ArrayAssignAST::ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name, ExprAST* element, ExprAST* value) : ASTNode(st) {}
