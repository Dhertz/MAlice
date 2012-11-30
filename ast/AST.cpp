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

	if (node) {
		node->print();
		
		vector< boost::shared_ptr<ASTNode> > children = node->getChildren();
		vector< boost::shared_ptr<ASTNode> >::iterator i;
		for (i = children.begin(); i != children.end(); ++i) {
			boost::shared_ptr<ASTNode> childNode = *i;
			if (!childNode) {
				cout << "node is null" << endl;
			} else {
				printNode(childNode, ++level);
			}
		}
		cout << endl;
	} else {
		cout << "node is null" << endl;
	}

}	
