#include "ArrayAssignAST.hpp"

ArrayAssignAST::ArrayAssignAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<ExprAST> element, boost::shared_ptr<ExprAST> value) : ASTNode(st) {}
