#include "ReturnAST.hpp"

ReturnAST::ReturnAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr) : ASTNode(st) {}
