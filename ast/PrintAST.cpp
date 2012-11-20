#include "PrintAST.hpp"

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, boost::shared_ptr<ExprAST> element) : ASTNode(st) {}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, boost::shared_ptr<CallParamsAST> params) : ASTNode(st) {}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {}
