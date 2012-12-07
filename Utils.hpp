#ifndef UTILS_H
#define UTILS_H

#include <antlr3commontree.h>
#include <string>
#include "AssemCom.hpp"
#include <vector>
#include <list>

using namespace std;

class Utils {
    static void print(pANTLR3_BASE_TREE tree, int level);
public:
    static int globalErrorCount;
    static void printTree(pANTLR3_BASE_TREE ast);
    static pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
    static string createStringFromTree(pANTLR3_BASE_TREE tree);
    static void printSemErr(int lineNo, string err);
    static void printSynErr(int lineNo, string err);
    static void printComErr(string err);
    static pair<string, vector<string> > tempForGlobal(string loc, 
    	vector<string> freeRegs, list<AssemCom>& instrs);
    static string borrowRegister(vector<string> args);
};

#endif
