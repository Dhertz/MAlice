#ifndef UTILS_H
#define UTILS_H

#include <antlr3commontree.h>
#include <string>

using namespace std;

class Utils {
    static void print(pANTLR3_BASE_TREE tree, int level);
public:
    static void printTree(pANTLR3_BASE_TREE ast);
    static pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
    static string createStringFromTree(pANTLR3_BASE_TREE tree);
    static void printSemErr(int lineNo, string err);
    static void printSynErr(int lineNo, string err);
};

#endif
