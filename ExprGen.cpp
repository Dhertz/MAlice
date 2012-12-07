#include "ExprGen.hpp"
#include "Utils.hpp"
#include "Label.hpp"
#include "idents/Array.hpp"
#include "idents/Variable.hpp"
#include <boost/lexical_cast.hpp>

typedef boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > treble_ptr_t;
typedef boost::tuple< string, list<AssemCom>, vector<string> > treble_t;

void printVector(vector<string> vec) {
	vector<string>::const_iterator i;
	for (i = vec.begin(); i != vec.end(); ++i) cout << *i << ' ';
	cout << endl;
}

string borrowRegister(vector<string> args);

treble_ptr_t ExprGen::generateExpression(pANTLR3_BASE_TREE root, boost::shared_ptr<SymbolTable> st, vector<string> freeRegs, boost::shared_ptr<AssemFunc> func) {
	/* Utils::printTree(root);
	printVector(freeRegs);
	cout << endl << endl << endl; */

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

			treble_ptr_t genParam = generateExpression(cp, st, freeRegs, func);
			string paramLoc = genParam->get<0>();
			list<AssemCom> paramInstrs = genParam->get<1>();
			freeRegs = genParam->get<2>();

			// Move the instructions to generate the param to the end of my
			//   rolling list of instructions
			instrs.splice(instrs.end(), paramInstrs);

			if (0 <= i && i < 4) {
				// First four arguments go in r0-r3 respectively
				if (paramLoc != "r" + boost::lexical_cast<string>(i)) {
					// Argument isn't already in the right place
					// mov r{i}, paramLoc
					vector<string> args;
					args.push_back("r" + boost::lexical_cast<string>(i));
					args.push_back(paramLoc);
					AssemCom mov("mov", args);
					instrs.push_back(mov);
				}
			} else {
				// Push any other params
				if (paramLoc[0] == 'r') {
					// paramLoc is already a register, so I can just push it
					// push paramloc

					vector<string> args;
					args.push_back("{" + paramLoc + "}");
					AssemCom push("push", args);
					instrs.push_back(push);
				} else if (freeRegs.empty()) {
					// Need to temporarily borrow a register (I've chosen r0)
					// push {r0}
					// mov r0, paramLoc
					// push {r0}
					// pop {r0}

					vector<string> args;
					args.push_back("{r0}");
					AssemCom push("push", args);
					instrs.push_back(push);

					args.clear();
					args.push_back("r0");
					args.push_back(paramLoc);
					AssemCom mov("mov", args);
					instrs.push_back(mov);

					// Re-use the push instruction from above
					instrs.push_back(push);

					args.clear();
					args.push_back("{r0}");
					AssemCom pop("pop", args);
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
					AssemCom mov("mov", args);
					instrs.push_back(mov);

					args.clear();
					args.push_back("{" + reg + "}");
					AssemCom push("push", args);
					instrs.push_back(push);
				}
			}
		}

		// bl funcName
		vector<string> args;
		args.push_back(funcName);
		AssemCom bl("bl", args);
		instrs.push_back(bl);

		treble_ptr_t ret(new treble_t("r0", instrs, freeRegs)); 
		return ret;
    } else if (tok == "VAR") {
        // Variable reference
        // Also allowed to be an array, so that function calls with array
        //   arguments are allowed

		// I think that I can just look up my ST until I find the Identifier,
		//   which will now have a string field of its assembly location
		// If this is set, I return it
		// If not, I allocate a register if possible, or memory space otherwise
		// Or perhaps it's an error if it's unallocated by this stage, see below

		string varName = Utils::createStringFromTree(Utils::childByNum(root, 0));
	    boost::shared_ptr<Identifier> varIdent = st->lookupCurrLevelAndEnclosingLevels(varName);

		if (varIdent->getBaseName() == "Type") {
            boost::shared_ptr<Type> varType = boost::shared_polymorphic_downcast<Type>(varIdent);
            boost::shared_ptr<Array> arr = boost::shared_polymorphic_downcast<Array>(varType);

			// Array will definitely have already been allocated by now
			string loc = arr->getAssLoc();
			treble_ptr_t ret(new treble_t(loc, instrs, freeRegs));
			return ret;
        } else {
            boost::shared_ptr<Variable> var = boost::shared_polymorphic_downcast<Variable>(varIdent);
			string loc = var->getAssLoc();

			if (loc == "") {
				// Allocate some space for the variable

				if (!freeRegs.empty()) {
					string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					var->setAssLoc(reg);

					treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
					return ret;
				} else {
					cout << "TODO: this case (~169 in ExprGen)" << endl;

					// mov rx, charByte
					vector<string> args;
					AssemCom mov("!", args);
					instrs.push_back(mov);

					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else {
				treble_ptr_t ret(new treble_t(loc, instrs, freeRegs));
				return ret;
			}
        }
    } else if (tok == "ARRMEMBER") {
        // Array member reference

        string arrName = Utils::createStringFromTree(Utils::childByNum(root, 0));
        boost::shared_ptr<Identifier> arrIdent = st->lookupCurrLevelAndEnclosingLevels(arrName);
		boost::shared_ptr<Type> arrType = boost::shared_polymorphic_downcast<Type>(arrIdent);
        boost::shared_ptr<Array> arr = boost::shared_polymorphic_downcast<Array>(arrType);

		string loc = arr->getAssLoc();

		pANTLR3_BASE_TREE index = Utils::childByNum(root, 1);

		treble_ptr_t genIndex = generateExpression(index, st, freeRegs, func);
		string indexLoc = genIndex->get<0>();
		list<AssemCom> indexInstrs = genIndex->get<1>();
		// Don't need to update freeRegs as long as indexLoc isn't used below

		// Move the instructions to generate the index to the end of my
		//   rolling list of instructions
		instrs.splice(instrs.end(), indexInstrs);

		// I imagine we'll have getAssSize() inside types, which will come in
		//   handy here when calculating the offset
		string elemType = arr->getElemType()->getTypeName();

		treble_ptr_t ret(new treble_t("*** array base loc " + loc + " offset for element " + indexLoc + " (elem type is " + elemType + ") ***", instrs, freeRegs));
		return ret;
    } else if (tok == "'") {
        // Char of form 'x'
		// Put it in a register if possible, otherwise somehow allocate memory

		string let = Utils::createStringFromTree(Utils::childByNum(root, 0));
		char letChar = let[0];
		int charByte = letChar; // TODO: check this is right

		if (!freeRegs.empty()) {
			// We have a free register, put the char in there
			string reg = freeRegs.front();
			freeRegs.erase(freeRegs.begin());

			// mov rx, charByte
			vector<string> args;
			args.push_back(reg);
			args.push_back("#" + boost::lexical_cast<string>(charByte));
			AssemCom mov("mov", args);
			instrs.push_back(mov);

			treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
			return ret;
		} else {
			cout << "TODO: this case (~227 in ExprGen)" << endl;
			treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
			return ret;
		}
    } else if (tok == "STR") {
        // String of form "foo", evaluates to a Sentence

    	string res = Utils::createStringFromTree(Utils::childByNum(root, 0));

		treble_ptr_t ret(new treble_t(res, instrs, freeRegs));
		return ret;
	} else if (tok == "EXPR") {
		pANTLR3_BASE_TREE expr = Utils::childByNum(root, 0);
		return generateExpression(expr, st, freeRegs, func);
    } else {
    	int children = root->getChildCount(root);
    	assert (0 <= children && children < 3);

    	if (children == 0) {
    	    // Number base case

    	    string n = Utils::createStringFromTree(root);
    	    if (!freeRegs.empty()) {
    	    	string reg = freeRegs.front();
    			freeRegs.erase(freeRegs.begin());

    			// mov rx, #n
    			vector<string> args;
    			args.push_back(reg);
    			args.push_back("#" + n);
    			AssemCom mov("mov", args);
    			instrs.push_back(mov);

    			treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    			return ret;
    	    } else {
    			cout << "TODO: this case (~282 in ExprGen)" << endl;
    			treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
    			return ret;
    	    }
    	} else if (children == 1) {
    		// Unary operator

    	    string op = Utils::createStringFromTree(root);
    	    pANTLR3_BASE_TREE arg = Utils::childByNum(root, 0);

    		treble_ptr_t argEval = generateExpression(arg, st, freeRegs, func);
    		string argLoc = argEval->get<0>();
    		list<AssemCom> argInstrs = argEval->get<1>();
    		freeRegs = argEval->get<2>();
    		instrs.splice(instrs.end(), argInstrs);

    		if (op == "!") {
    			vector<string> args;
    			string reg, stackLoc, retLoc;

			    if (!freeRegs.empty()) {
			    	reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					int currPtr = func->getStackPointer() + 4;
					stackLoc = "[fp,#-" + boost::lexical_cast<string>(currPtr) + "]";
					func->increaseStackPointer(4);

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(argLoc);
					reg = borrowRegister(argRegs);

					// push {reg}
					args.push_back("{" + reg + "}");
					AssemCom push("push", args);
					instrs.push_back(push);
				}

				// borrow a register for #0xFFFFFFFF
				args.clear();
				args.push_back(reg);
				args.push_back(argLoc);
				string tempReg = borrowRegister(args);

				// push {tempReg}
				// mov tempReg, #0xFFFFFFFF
				args.clear();
				args.push_back("{" + tempReg + "}");
				AssemCom push("push", args);
				instrs.push_back(push);

				args.clear();
				args.push_back(tempReg);
				args.push_back("#0xFFFFFFFF");
				AssemCom mov("mov", args);
				instrs.push_back(mov);

				// eor reg, argLoc, tempReg
				args.clear();
				args.push_back(reg);
				args.push_back(argLoc);
				args.push_back(tempReg);
				AssemCom eor("eor", args);
				instrs.push_back(eor);

				// pop {tempReg}
				args.clear();
				args.push_back("{" + tempReg + "}");
				AssemCom popTemp("pop", args);
				instrs.push_back(popTemp);

			    if (!freeRegs.empty()) {
			    	retLoc = reg;
				} else {
					// str reg, [fp, #-4]
					args.clear();
					args.push_back(reg);
					args.push_back(stackLoc);
					AssemCom str("str", args);
					instrs.push_back(str);

					// pop {reg}
					args.clear();
					args.push_back("{" + reg + "}");
					AssemCom pop("pop", args);
					instrs.push_back(pop);

					retLoc = stackLoc;
				}

				treble_ptr_t ret(new treble_t(retLoc, instrs, freeRegs));
				return ret;
    		} else if (op == "~") {
			    if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// mvn reg, argLoc
					vector<string> args;
					args.push_back(reg);
					args.push_back(argLoc);
					AssemCom mvn("mvn", args);
					instrs.push_back(mvn);

					treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
					return ret;
				} else {
					cout << "TODO: this case (~317 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
    		} else if (op == "+" || op == "-") {
			    if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// unary +:
					// mov reg, argLoc
    	        	// cmp reg, #0
    				// neglt reg, reg

					// unary -:
					// mov reg, argLoc
					// cmp reg, #0
    				// neggt reg, reg

    				vector<string> args;
    				args.push_back(reg);
    				args.push_back(argLoc);
    				AssemCom mov("mov", args);
    				instrs.push_back(mov);

    				args.clear();
    				args.push_back(reg);
    				args.push_back("#0");
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

    				string negInstr = (op == "+") ? "neglt" : "neggt";

    				args.clear();
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom neg(negInstr, args);
    				instrs.push_back(neg);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~317 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
    	    }
    	} else if (children == 2) {
			// Binary operator

		    string op = Utils::createStringFromTree(root);

		    pANTLR3_BASE_TREE lhs = Utils::childByNum(root, 0);
			treble_ptr_t lhsEval = generateExpression(lhs, st, freeRegs, func);
			string lhsLoc = lhsEval->get<0>();
			list<AssemCom> lhsInstrs = lhsEval->get<1>();
			freeRegs = lhsEval->get<2>();
			instrs.splice(instrs.end(), lhsInstrs);

			pANTLR3_BASE_TREE rhs = Utils::childByNum(root, 1);
			treble_ptr_t rhsEval = generateExpression(rhs, st, freeRegs, func);
			string rhsLoc = rhsEval->get<0>();
			list<AssemCom> rhsInstrs = rhsEval->get<1>();
			freeRegs = rhsEval->get<2>();
			instrs.splice(instrs.end(), rhsInstrs);

			if (op == "||" || op == "|") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// orr reg, lhsLoc, rhsLoc
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom orr("orr", args);
    				instrs.push_back(orr);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~396 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "&&" || op == "&") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// and reg, lhsLoc, rhsLoc
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom andInstr("and", args);
    				instrs.push_back(andInstr);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~417 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "^") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// eor reg, lhsLoc, rhsLoc
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom eor("eor", args);
    				instrs.push_back(eor);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~441 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "+") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// add reg, lhsLoc, rhsLoc
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom add("add", args);
    				instrs.push_back(add);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~461 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "-") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// sub reg, lhsLoc, rhsLoc
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom sub("sub", args);
    				instrs.push_back(sub);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~481 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "*") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// mul reg, lhsLoc, rhsLoc
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom mul("mul", args);
    				instrs.push_back(mul);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~501 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "/") {
				if (!freeRegs.empty()) {
			    	string res = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
					if (!freeRegs.empty()) {
						string reg = freeRegs.front();
						freeRegs.erase(freeRegs.begin());

					vector<string> args;
    				args.push_back(res);
    				args.push_back(res);
    				args.push_back(res);
    				AssemCom xors("xor", args);
    				instrs.push_back(xors);

    				Label l;
					instrs.push_back(AssemCom(l.getLabel() + 
											":", std::vector<string>()));

    				args.clear();
					args.push_back(reg);
					args.push_back(lhsLoc);
	    			AssemCom mov("mov", args);
	    			instrs.push_back(mov);

					args.clear();
					args.push_back(reg);
					args.push_back(reg);
	    			args.push_back(rhsLoc);
	    			AssemCom sub("sub", args);
	    			instrs.push_back(sub);
	    			
	    			args.clear();
	    			args.push_back(res);
					args.push_back(res);
	    			args.push_back("#1");
	    			AssemCom add("add", args);
	    			instrs.push_back(add);

	    			args.clear();
					args.push_back(reg);
					args.push_back(rhsLoc);
					AssemCom cmp("cmp", args);
					instrs.push_back(cmp);

					args.clear();
					args.push_back(l.getLabel());
					AssemCom blge("blge", args);
					instrs.push_back(blge);

					freeRegs.push_back(res);

    				treble_ptr_t ret(new treble_t(res, instrs, freeRegs));
    				return ret;
					
					} else {
						cout << "TODO: this case (~560 in ExprGen)" << endl;
						treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
						return ret;
					}

				} else {
					cout << "TODO: this case (~567 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "%") {
				if (!freeRegs.empty()) {
					Label l;
					instrs.push_back(AssemCom(l.getLabel() + 
										":", std::vector<string>()));
					string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					vector<string> args;
					args.push_back(reg);
					args.push_back(lhsLoc);
	    			AssemCom mov("mov", args);
	    			instrs.push_back(mov);

					args.clear();
					args.push_back(reg);
					args.push_back(reg);
	    			args.push_back(rhsLoc);
	    			AssemCom sub("sub", args);
	    			instrs.push_back(sub);

					args.clear();
					args.push_back(reg);
					args.push_back(rhsLoc);
					AssemCom cmp("cmp", args);
					instrs.push_back(cmp);

					args.clear();
					args.push_back(l.getLabel());
					AssemCom blge("blge", args);
					instrs.push_back(blge);

					treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
					return ret;
				} else {
					cout << "TODO: this case (~592 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "==") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// eor reg, reg, reg
					// cmp lhsLoc, rhsLoc
					// moveq reg, #0xFFFFFFFF
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom eor("eor", args);
    				instrs.push_back(eor);

    				args.clear();
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

    				args.clear();
    				args.push_back(reg);
    				args.push_back("#0xFFFFFFFF");
    				AssemCom moveq("moveq", args);
    				instrs.push_back(moveq);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~538 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "!=") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// mov reg, #0xFFFFFFFF
					// cmp lhsLoc, rhsLoc
					// eoreq reg, reg, reg

					vector<string> args;
    				args.push_back(reg);
    				args.push_back("#0xFFFFFFFF");
    				AssemCom mov("mov", args);
    				instrs.push_back(mov);

    				args.clear();
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

    				args.clear();
    				args.push_back(reg);
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom eoreq("eoreq", args);
    				instrs.push_back(eoreq);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~572 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == ">") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// eor reg, reg, reg
					// cmp lhsLoc, rhsLoc
					// movgt reg, #0xFFFFFFFF
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom eor("eor", args);
    				instrs.push_back(eor);

    				args.clear();
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

    				args.clear();
    				args.push_back(reg);
    				args.push_back("#0xFFFFFFFF");
    				AssemCom movgt("movgt", args);
    				instrs.push_back(movgt);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~601 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "<") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// eor reg, reg, reg
					// cmp lhsLoc, rhsLoc
					// movlt reg, #0xFFFFFFFF
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom eor("eor", args);
    				instrs.push_back(eor);

    				args.clear();
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

    				args.clear();
    				args.push_back(reg);
    				args.push_back("#0xFFFFFFFF");
    				AssemCom movlt("movlt", args);
    				instrs.push_back(movlt);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~634 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == ">=") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// eor reg, reg, reg
					// cmp lhsLoc, rhsLoc
					// movge reg, #0xFFFFFFFF
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom eor("eor", args);
    				instrs.push_back(eor);

    				args.clear();
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

    				args.clear();
    				args.push_back(reg);
    				args.push_back("#0xFFFFFFFF");
    				AssemCom movge("movge", args);
    				instrs.push_back(movge);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~667 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			} else if (op == "<=") {
				if (!freeRegs.empty()) {
			    	string reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

					// eor reg, reg, reg
					// cmp lhsLoc, rhsLoc
					// movle reg, #0xFFFFFFFF
					vector<string> args;
    				args.push_back(reg);
    				args.push_back(reg);
    				args.push_back(reg);
    				AssemCom eor("eor", args);
    				instrs.push_back(eor);

    				args.clear();
    				args.push_back(lhsLoc);
    				args.push_back(rhsLoc);
    				AssemCom cmp("cmp", args);
    				instrs.push_back(cmp);

					args.clear();
    				args.push_back(reg);
    				args.push_back("#0xFFFFFFFF");
    				AssemCom movle("movle", args);
    				instrs.push_back(movle);

    				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    				return ret;
				} else {
					cout << "TODO: this case (~700 in ExprGen)" << endl;
					treble_ptr_t ret(new treble_t("TODO", instrs, freeRegs));
					return ret;
				}
			}
    	}
    }
}

int ExprGen::evaluateExpression(pANTLR3_BASE_TREE root, boost::shared_ptr<SymbolTable> st) {
	string tok = Utils::createStringFromTree(root);

	if (tok == "VAR") {
		string varName = Utils::createStringFromTree(Utils::childByNum(root, 0));
	    boost::shared_ptr<Identifier> varIdent = st->lookupCurrLevelAndEnclosingLevels(varName);

		if (varIdent->getBaseName() != "Type") {
            boost::shared_ptr<Variable> var = boost::shared_polymorphic_downcast<Variable>(varIdent);
            if (var->hasEvaluatedVal())
            	return var->getVal();
        }
	} else if (tok == "EXPR") {
		pANTLR3_BASE_TREE expr = Utils::childByNum(root, 0);
		return evaluateExpression(expr, st);
    } else {
    	int children = root->getChildCount(root);
    	assert (0 <= children && children < 3);

    	if (children == 0) {
    	    // Number base case

			string n = Utils::createStringFromTree(root);
    	    return atoi(n.c_str());
    	} else if (children == 1) {
    		// Unary operator

    	    string op = Utils::createStringFromTree(root);
    	    int arg = evaluateExpression(Utils::childByNum(root, 0), st);

    		if (op == "~") {
    			return ~arg;
    		} else if (op == "+") {
    			return +arg;
    	    } else if (op == "-") {
    	    	return -arg;
    	    }
    	} else if (children == 2) {
			// Binary operator

		    string op = Utils::createStringFromTree(root);

		    int lhs = evaluateExpression(Utils::childByNum(root, 0), st);
		    int rhs = evaluateExpression(Utils::childByNum(root, 1), st);

			if (op == "|") {
				return lhs | rhs;
			} else if (op == "&") {
				return lhs & rhs;
			} else if (op == "^") {
				return lhs ^ rhs;
			} else if (op == "+") {
				return lhs + rhs;
			} else if (op == "-") {
				return lhs - rhs;
			} else if (op == "*") {
				return lhs * rhs;
			} else if (op == "/") {
				return lhs / rhs;
			} else if (op == "%") {
				return lhs % rhs;
			}
    	}
    }

    Utils::printComErr("Could not evaluate array size.");
}

// Returns a register that isn't in args, used in stack storage case
string borrowRegister(vector<string> args) {
	string regs[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};
	vector<string> allRegs =
		vector<string>(regs, regs + sizeof(regs) / sizeof(string));

	for (vector<string>::iterator it = allRegs.begin(); it != allRegs.end(); ++it) {
		if (find(args.begin(), args.end(), *it) == args.end())
			return *it;
	}
	// This won't ever be hit, as no case above uses so many arguments
	return "";
}
