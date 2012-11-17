#include "PrintAST.hpp"

PrintAST::PrintAST(SymbolTable* st, string arrayName, ExprAST* element) : ASTNode(st) {}

PrintAST::PrintAST(SymbolTable* st, string funcName, CallParamsAST* params) : ASTNode(st) {}

PrintAST::PrintAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {}
