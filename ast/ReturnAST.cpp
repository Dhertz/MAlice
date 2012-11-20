#include "ReturnAST.hpp"

ReturnAST::ReturnAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {}
