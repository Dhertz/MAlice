#ifndef FUNC_DEC_AST
#define FUNC_DEC_AST

#include "HeaderParamsAST.hpp"
#include "../idents/Function.hpp"
#include "../ASTVisitor.hpp"

class FuncDecAST : public ASTNode {
    boost::shared_ptr<SymbolTable> _st;
    string _name;
    string _returnType;
    boost::shared_ptr<HeaderParamsAST> _params;
    boost::shared_ptr<Function> _funcObj;
    int _lineNo;
    void check();
public:
    FuncDecAST(boost::shared_ptr<SymbolTable> st, string name,
                 boost::shared_ptr<HeaderParamsAST> params, string returnType,
                 boost::weak_ptr<ASTNode> parent, int lineNo);
    string getNodeName();
    string getFuncName();
    void print();
    void accept(boost::shared_ptr<ASTVisitor> v);
    string checkFunctionName(string name, boost::shared_ptr<SymbolTable> st);
};

#endif
