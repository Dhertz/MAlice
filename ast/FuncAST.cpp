#include "FuncAST.hpp"

FuncAST::FuncAST(boost::shared_ptr<SymbolTable> st, string name, CallParamsAST* params) : ASTNode(st) {}
