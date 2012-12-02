#include "ExprGen.hpp"
#include "Utils.hpp"

pair< string, list<AssemCom> > generateExpression(pANTLR3_BASE_TREE root, boost::shared_ptr<SymbolTable> st) {
	// These should be shared with ExprAST
	set<string> boolArgBoolRet;
    set<string> intArgIntRet;
    set<string> boolArgsBoolRet;
    set<string> mixedArgsMixedRet;
    set<string> mixedArgsBoolRet;
	boolArgBoolRet.insert("!");
    intArgIntRet.insert("~");
    intArgIntRet.insert("+");
    intArgIntRet.insert("-");
    boolArgsBoolRet.insert("||");
    boolArgsBoolRet.insert("&&");
    mixedArgsMixedRet.insert("|");
    mixedArgsMixedRet.insert("^");
    mixedArgsMixedRet.insert("&");
    mixedArgsMixedRet.insert("&");
    mixedArgsMixedRet.insert("&");
    mixedArgsMixedRet.insert("+");
    mixedArgsMixedRet.insert("-");
    mixedArgsMixedRet.insert("*");
    mixedArgsMixedRet.insert("/");
    mixedArgsMixedRet.insert("%");
    mixedArgsBoolRet.insert("==");
    mixedArgsBoolRet.insert("!=");
    mixedArgsBoolRet.insert(">");
    mixedArgsBoolRet.insert("<");
    mixedArgsBoolRet.insert(">=");
    mixedArgsBoolRet.insert("<=");

	string tok = Utils::createStringFromTree(root);

    if (tok == "FUNC") {
        // Inline function call, evaluates to function's return type

        string funcName = Utils::createStringFromTree(Utils::childByNum(root, 0));
        pANTLR3_BASE_TREE cplTree = Utils::childByNum(root, 1);

		for (int i = 0; i < cplTree->getChildCount(cplTree); ++i) {
			pANTLR3_BASE_TREE cp = Utils::childByNum(cplTree, i);
			pair< string, list<AssemCom> > genParam = generateExpression(cp, st);
		}

    } else if (tok == "VAR") {
        // Variable reference, evaluates to variable's type
        // Also allowed to be an array, so that function calls with array
        //   arguments are allowed

    } else if (tok == "ARRMEMBER") {
        // Array member reference, evaluates to array's element type

    } else if (tok == "'") {
        // Char of form 'x', evaluates to a Letter

    } else if (tok == "\"") {
        // String of form "foo", evaluates to a Sentence

    } else {
        // Recursive case, will resolve to a Boolean or Number

    }
}
