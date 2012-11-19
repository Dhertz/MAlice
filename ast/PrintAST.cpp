#include "PrintAST.hpp"

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, ExprAST* element) : ASTNode(st) {}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, CallParamsAST* params) : ASTNode(st) {}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr) : ASTNode(st) {}
