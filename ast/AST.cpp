#include "AST.hpp"

void AST::setRoot(ASTNode* root) {
	_root = root;
}

void AST::print() {
	printNode(_root);
}

void AST::printNode(ASTNode* node) {
	node->print();

	vector<ASTNode*>::iterator i;

	for (i = node->getChildren().begin(); i < node->getChildren().end(); i++) {
		printNode(*i);
	}
}
