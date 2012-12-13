#include "Utils.hpp"
#include <iostream>
#include <algorithm>

int Utils::globalErrorCount = 0;

void Utils::print(pANTLR3_BASE_TREE tree, int level) {
    for (int i = 0; i < level; ++i) {
        cout << "--";
    }

    cout << " " << tree->getText(tree)->chars << endl;

    int childCount = tree->getChildCount(tree);
    for (int i = 0; i < childCount; ++i) {
        print((pANTLR3_BASE_TREE) tree->getChild(tree, i), level + 1);
    }
}

void Utils::printTree(pANTLR3_BASE_TREE ast) {
    print(ast, 0);
}

pANTLR3_BASE_TREE Utils::childByNum(pANTLR3_BASE_TREE tree, int num) {
    return (pANTLR3_BASE_TREE) tree->getChild(tree, num);
}

string Utils::createStringFromTree(pANTLR3_BASE_TREE tree) {
    string res(
    	(const char *) tree->getText(tree)->to8(tree->getText(tree))->chars,
        tree->getText(tree)->len);
    return res;
}

void Utils::printSemErr(int lineNo, string err) {
    cerr << "Semantic error, line " << lineNo << ": " << err << endl;
    globalErrorCount++;
}

void Utils::printSynErr(int lineNo, string err) {
    cerr << "Syntax error, line " << lineNo << ": " << err << endl;
    globalErrorCount++;
}

void Utils::printComErr(string err) {
    cerr << "Compile error: " << err << endl << endl;
    exit(1);
}

// make a temporary copy of a global variable in a register
pair<string, vector<string> > Utils::tempForGlobal(string loc, 
	vector<string> freeRegs, list<AssemCom>& instrs) {
	string tmp = freeRegs.front();
	freeRegs.erase(freeRegs.begin());
	// remember to "free" the register once it has been finished with

	vector<string> args;
	args.push_back(tmp);
	args.push_back(loc);
	AssemCom ldr("ldr", args);
	instrs.push_back(ldr);

	pair<string, vector<string> > p = make_pair(tmp, freeRegs);

	return p;
}

// Returns a register that isn't in args, used in stack storage case
string Utils::borrowRegister(vector<string> args) {
	string regs[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};
	vector<string> allRegs =
		vector<string>(regs, regs + sizeof(regs) / sizeof(string));

	for (vector<string>::iterator it = allRegs.begin(); it != allRegs.end(); 
																		++it) {
		if (find(args.begin(), args.end(), *it) == args.end())
			return *it;
	}
	// This won't ever be hit, since no function in ASTVisitor or ExprGen 
	// requires all the registers concretely
	return "";
}
