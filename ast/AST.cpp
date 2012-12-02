#include "AST.hpp"

void AST::setRoot(boost::shared_ptr<ProgAST> root) {
    _root = root;
}

void AST::print() {
	printNode(_root, 0);
}

boost::shared_ptr<ProgAST> AST::getRoot() {
	return _root;
}

void AST::printNode(boost::shared_ptr<ASTNode> node, int level) {
	for (int i = 0; i < level; ++i) {
        cout << "--";
    }

    cout << " ";
    node->print();

	vector< boost::shared_ptr<ASTNode> > children = node->getChildren();
	vector< boost::shared_ptr<ASTNode> >::iterator i;
	for (i = children.begin(); i != children.end(); ++i) {
		boost::shared_ptr<ASTNode> childNode = *i;
		printNode(childNode, level + 1);
	}
}	
