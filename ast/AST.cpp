#include "AST.hpp"

void AST::setRoot(boost::shared_ptr<ASTNode> root) {
    _root = root;
}

void AST::print() {
	printNode(_root, 0);
}

void AST::printNode(boost::shared_ptr<ASTNode> node, int level) {
	for (int i = 0; i < level; ++i) {
        cout << "--";
    }

	node->print();
	vector< boost::shared_ptr<ASTNode> >::iterator i;
	for (i = node->getChildren().begin(); i != node->getChildren().end(); ++i) {
		if (!(*i)) {
			cout << "node is null" << endl;
		} else {
			printNode(*i, ++level);
		}
	}
	cout << endl;
}	
