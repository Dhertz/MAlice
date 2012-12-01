#ifndef PROC_DEC_AST
#define PROC_DEC_AST

#include "HeaderParamsAST.hpp"
#include "../idents/Proc.hpp"
#include "../ASTVisitor.hpp"

class ProcDecAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _name;
    string _returnType;
    boost::shared_ptr<HeaderParamsAST> _params;
    boost::shared_ptr<Proc> _procObj;
    int _lineNo;
    void check();
public:
    ProcDecAST(boost::shared_ptr<SymbolTable> st, string name,
    	         boost::shared_ptr<HeaderParamsAST> params,
    	         boost::weak_ptr<ASTNode> parent, int lineNo);
    string getProcName();
    string getNodeName();
    void print();
    void accept(boost::shared_ptr<ASTVisitor> v);
};

#endif
