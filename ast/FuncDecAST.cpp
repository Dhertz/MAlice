#include "FuncDecAST.hpp"

FuncDecAST::FuncDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params, string returnType) : ASTNode(st) {}
