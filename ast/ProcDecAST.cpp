#include "ProcDecAST.hpp"

ProcDecAST::ProcDecAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<HeaderParamsAST> params) : ASTNode(st) {}
