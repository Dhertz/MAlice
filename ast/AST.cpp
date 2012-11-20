#include "AST.hpp"

void AST::setRoot(boost::shared_ptr<ASTNode> root) {
	_root = root;
}

/*void AST::print() {
	printNode(_root);
}

void AST::printNode(boost::shared_ptr<ASTNode> node) {
	node->print();

	vector< boost::shared_ptr<ASTNode> >::iterator i;

	for (i = node->getChildren().begin(); i < node->getChildren().end(); i++) {
		printNode(i);
	}
}
*/