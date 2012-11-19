#include "FuncDecAST.hpp"

FuncDecAST::FuncDecAST(boost::shared_ptr<SymbolTable> st, string name, HeaderParamsAST* params, string returnType) : ASTNode(st) {}
