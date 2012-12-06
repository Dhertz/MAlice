#include "Utils.hpp"
#include <iostream>

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
    string res((const char *) tree->getText(tree)->to8(tree->getText(tree))->chars,
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
