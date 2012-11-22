#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include <antlr3commontree.h>
#include <string>

using namespace std;

class TreeUtils {
public:
	static pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	static string createStringFromTree(pANTLR3_BASE_TREE tree);
};

#endif
