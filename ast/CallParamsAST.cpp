#include "CallParamsAST.hpp"
#include "ExprAST.hpp"
#include "../Utils.hpp"

CallParamsAST::CallParamsAST(boost::shared_ptr<SymbolTable> st,
                               pANTLR3_BASE_TREE tree,
                               boost::shared_ptr<ASTNode> parent, int lineNo)
                               : ASTNode(st, parent, lineNo) {
    _st = st;
    _tree = tree;
    _parent = parent;
    _lineNo = lineNo;
    check();
}

vector< boost::shared_ptr<Type> > CallParamsAST::getTypes() {
    return _types;
}

void CallParamsAST::check() {
    for (int i = 0; i < _tree->getChildCount(_tree); ++i) {
        ExprAST e(_st, Utils::childByNum(_tree, i), _parent, _lineNo, true);
        if (!e.getType()) {
            Utils::printSemErr(_lineNo, (string) "Invalid expression in " +
                                 "function call due to earlier error.");
        } else {
            _types.push_back(e.getType());
        }
    }
}
