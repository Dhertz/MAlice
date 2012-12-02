#include "ExprGen.hpp"
#include "Utils.hpp"

boost::tuple< string, list<AssemCom>, vector<string> > ExprGen::generateExpression(pANTLR3_BASE_TREE root, boost::shared_ptr<SymbolTable> st, vector<string> freeRegs) {
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
	list<AssemCom> instrs;

    if (tok == "FUNC") {
        // Inline function call
		// Basic idea here is to evaluate all arguments, put them in the right
		//   place if they aren't already, make the call (need to make sure
		//   the label matches the one made in the function declaration), and
		//   I know my result will be in r0 by convention

        string funcName = Utils::createStringFromTree(Utils::childByNum(root, 0));
        pANTLR3_BASE_TREE cplTree = Utils::childByNum(root, 1);

		for (int i = 0; i < cplTree->getChildCount(cplTree); ++i) {
			pANTLR3_BASE_TREE cp = Utils::childByNum(cplTree, i);

			boost::tuple< string, list<AssemCom>, vector<string> > genParam = generateExpression(cp, st, freeRegs);
			string paramLoc = genParam.get<0>();
			list<AssemCom> paramInstrs = genParam.get<1>();
			// Don't need to update freeRegs because none of the temporary
			//   values that might be in the registers need to be kept, as long
			//   as I don't lose my argument value

			// Move the instructions to generate the param to the end of my
			//   rolling list of instructions
			instrs.splice(instrs.end(), paramInstrs);

			if (0 <= i && i < 4) {
				// First four arguments go in r0-r3 respectively
				if (paramLoc != ("r" + i)) {
					// Argument isn't already in the right place
					// mov r{i}, paramLoc
					vector<string> args;
					args.push_back("r" + i);
					args.push_back(paramLoc);
					AssemCom mov("mov", args.size(), args);
					instrs.push_back(mov);
				}
			} else {
				// Push any other params
				if (freeRegs.empty()) {
					// Need to temporarily borrow a register (I've chosen r0)
					// push {r0}
					// mov r0, paramLoc
					// push {r0}
					// pop {r0}

					vector<string> args;
					args.push_back("{r0}");
					AssemCom push("push", args.size(), args);
					instrs.push_back(push);

					args.clear();
					args.push_back("r0");
					args.push_back(paramLoc);
					AssemCom mov("mov", args.size(), args);
					instrs.push_back(mov);

					// Need to check this works how I want it to
					// i.e. re-use the push instruction above
					instrs.push_back(push);

					args.clear();
					args.push_back("{r0}");
					AssemCom pop("pop", args.size(), args);
					instrs.push_back(pop);
				} else {
					// Use first free register to save need for extra push/pop
					// Don't need to update freeRegs though, because I don't
					//   care about keeping the value
					// mov rx, paramLoc
					// push rx

					string reg = freeRegs.front();

					vector<string> args;
					args.push_back(reg);
					args.push_back(paramLoc);
					AssemCom mov("mov", args.size(), args);
					instrs.push_back(mov);

					args.clear();
					args.push_back("{" + reg + "}");
					AssemCom push("push", args.size(), args);
					instrs.push_back(push);
				}
			}
		}
		return boost::tuple< string, list<AssemCom>, vector<string> >("r0", instrs, freeRegs);
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
