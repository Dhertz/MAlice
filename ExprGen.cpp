#include "ExprGen.hpp"
#include "Utils.hpp"
#include "Label.hpp"
#include "idents/Array.hpp"
#include "idents/Variable.hpp"
#include <boost/lexical_cast.hpp>
#include <algorithm>

typedef boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > treble_ptr_t;
typedef boost::tuple< string, list<AssemCom>, vector<string> > treble_t;

void printVector(vector<string> vec) {
	vector<string>::const_iterator i;
	for (i = vec.begin(); i != vec.end(); ++i) cout << *i << ' ';
	cout << endl;
}

map<int, string> ExprGen::_constRegs;

/*
	Generate the intructions required to create an expression
	Returns the register containing the final result, the list of instructions
		generated and the updated list of free registers
*/
treble_ptr_t ExprGen::generateExpression(pANTLR3_BASE_TREE root, 
											boost::shared_ptr<SymbolTable> st, 
											vector<string> freeRegs, 
											boost::shared_ptr<AssemFunc> func) {

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

        int maxpush = 3;
        if(!freeRegs.empty()) {
			maxpush = min(freeRegs.front()[1] - 48, 3);
		}

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

				if (paramLoc[0] != 'r') {
					string tempReg = Utils::borrowRegister(vector<string>());

					addCommand(instrs, "ldr", tempReg, paramLoc);
					
					paramLoc = tempReg;
				}

				// First four arguments go in r0-r3 respectively
				if (paramLoc != "r" + boost::lexical_cast<string>(i)) {
					// Argument isn't already in the right place
					// mov r{i}, paramLoc
					addCommand(instrs, "mov", 
								"r" + boost::lexical_cast<string>(i), paramLoc);
				}

		    	for (int i = 1; i < maxpush; ++i) {
		    		addCommand(instrs, "push", 
		    					"{r" + boost::lexical_cast<string>(i) + "}");
		    	}
			} else {
				// Push any other params
				if (paramLoc[0] == 'r') {
					// paramLoc is already a register, so I can just push it
					// push paramloc

					addCommand(instrs, "push", "{" + paramLoc + "}");
				} else if (freeRegs.empty()) {
					// Need to temporarily borrow a register (I've chosen r0)
					// push {r0}
					// mov r0, paramLoc
					// push {r0}
					// pop {r0}

					// This doesn't look right to me - Owen

					addCommand(instrs, "push", "{r0}");
					addCommand(instrs, "mov", "r0", paramLoc);
					addCommand(instrs, "push", "{r0}");
					addCommand(instrs, "pop", "{r0}");
				} else {
					// Use first free register to save need for extra push/pop
					// Don't need to update freeRegs though, because I don't
					//   care about keeping the value
					// mov rx, paramLoc
					// push rx

					string reg = freeRegs.front();

					addCommand(instrs, "mov", reg, paramLoc);
					addCommand(instrs, "push", "{" + reg + "}");
				}
			}
		}

		// bl funcName
		addCommand(instrs, "bl", funcName);

		for (int j = maxpush - 1; j >= 1; --j) {
			addCommand(instrs, "pop", 
						"{r" + boost::lexical_cast<string>(j) + "}");
		}

		treble_ptr_t ret(new treble_t("r0", instrs, freeRegs)); 
		return ret;
    } else if (tok == "VAR") {
        // Variable reference
        // Also allowed to be an array, so that function calls with array
        //   arguments are allowed

		// I can just look up my ST until I find the Identifier,
		//   which will now have a string field of its assembly location
		// If this is set, I return it
		// If not, I allocate a register if possible, or memory space otherwise

		string varName = Utils::createStringFromTree(Utils::childByNum(root, 0));
	    boost::shared_ptr<Identifier> varIdent = st->lookupCurrLevelAndEnclosingLevels(varName);

		if (varIdent->getBaseName() == "Type") {
            boost::shared_ptr<Type> varType = boost::shared_polymorphic_downcast<Type>(varIdent);
            boost::shared_ptr<Array> arr = boost::shared_polymorphic_downcast<Array>(varType);

			// Array will definitely have been allocated by now
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
					func->increaseStackPointer(4);
					string stackLoc = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";

					var->setAssLoc(stackLoc);

					treble_ptr_t ret(new treble_t(stackLoc, instrs, freeRegs));
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

		string arrayLoc = arr->getAssLoc();

		pANTLR3_BASE_TREE index = Utils::childByNum(root, 1);

		treble_ptr_t genIndex = generateExpression(index, st, freeRegs, func);
		string indexLoc = genIndex->get<0>();
		list<AssemCom> indexInstrs = genIndex->get<1>();

		// Move the instructions to generate the index to the end of my
		//   rolling list of instructions
		instrs.splice(instrs.end(), indexInstrs);

		bool onStack = false;
		string oldLoc = indexLoc;
		if (indexLoc[0] != 'r') {
			// result is stored on the stack
			onStack = true;
			addCommand(instrs, "push", "{r7}");
			addCommand(instrs, "ldr", "r7", indexLoc);
			indexLoc = "r7";
		}

		addCommand(instrs, "mov", indexLoc, indexLoc, "LSL #2");
		if (arrayLoc[0] == '[') {
			// local array
			int arrayStartIndex = atoi(arrayLoc.substr(arrayLoc.find("-") + 1).c_str());

			addCommand(instrs, "sub", indexLoc, indexLoc, "#" + boost::lexical_cast<string>(arrayStartIndex));
			addCommand(instrs, "add", indexLoc, "fp", indexLoc);
			addCommand(instrs, "ldr", indexLoc, "[" + indexLoc + "]");
		} else if (arrayLoc[0] == '.') {
			// global array
			string reg = Utils::borrowRegister(vector<string>(1, indexLoc));
			vector<string> args;
			args.push_back(reg);
			args.push_back(indexLoc);
			oldLoc = Utils::borrowRegister(args);

			addCommand(instrs, "ldr", reg, arrayLoc);
			addCommand(instrs, "ldr", oldLoc, "[" + reg + ", " + indexLoc + "]");
		} else {
			// reference
			addCommand(instrs, "ldr", oldLoc, "[" + arrayLoc + ", " + indexLoc + "]");
		}

		if (onStack) {
			// pop {reg}
			addCommand(instrs, "str", "r7", oldLoc);
			addCommand(instrs, "pop", "{r7}");
		}

		treble_ptr_t ret(new treble_t(oldLoc, instrs, freeRegs));
		return ret;
    } else if (tok == "'") {
        // Char of form 'x'
		// Put it in a register if possible, otherwise save onto stack

		string let = Utils::createStringFromTree(Utils::childByNum(root, 0));
		char letChar = let[0];
		int charByte = letChar;

		if (!freeRegs.empty()) {
			// We have a free register, put the char in there
			string reg = freeRegs.front();
			freeRegs.erase(freeRegs.begin());

			// mov rx, charByte
			addCommand(instrs, "mov", reg, 
						"#" + boost::lexical_cast<string>(charByte));

			treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
			return ret;
		} else {
			func->increaseStackPointer(4);
			string stackLoc = "[fp, #-" + 
				boost::lexical_cast<string>(func->getStackPointer()) + "]";

			addCommand(instrs, "ldr", stackLoc, 
						"#" + boost::lexical_cast<string>(charByte));

			treble_ptr_t ret(new treble_t(stackLoc, instrs, freeRegs));
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

    	if (children == 0) {
    	    // Number base case

    	    string n = Utils::createStringFromTree(root);
    	    if (!freeRegs.empty()) {
    	    	string reg = freeRegs.front();
    			freeRegs.erase(freeRegs.begin());

				if (atoi(n.c_str()) > 255) {
					// To large a value to use mov, use ldr instead
					// ldr rx, =#n
					addCommand(instrs, "ldr", reg, "=" + n);
				} else {
	    			// mov rx, #n
	    			addCommand(instrs, "mov", reg, "#" + n);
				}

    			treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    			return ret;
    	    } else {
    	    	func->increaseStackPointer(4);
    	    	string stackLoc = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";
				
				addCommand(instrs, "push", "{r0}");
				addCommand(instrs, "mov", "r0", "#" + n);
				addCommand(instrs, "str", "r0", stackLoc);
				addCommand(instrs, "pop", "{r0}");

    			treble_ptr_t ret(new treble_t(stackLoc, instrs, freeRegs));
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

			string argTempLoc;
			bool argOnStack = false;
			if (argLoc[0] != 'r') {
				argOnStack = true;
				addCommand(instrs, "push", "{r4}");
				addCommand(instrs, "ldr", "r4", argLoc);

				argTempLoc = argLoc;
				argLoc = "r4";
			}

    		if (op == "!") {
    			vector<string> args;
    			string reg, stackLoc, retLoc;

			    if (!freeRegs.empty()) {
			    	reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					int currPtr = func->getStackPointer() + 4;
					stackLoc = "[fp, #-" + boost::lexical_cast<string>(currPtr) + "]";
					func->increaseStackPointer(4);

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(argLoc);
					reg = Utils::borrowRegister(argRegs);

					// push {reg}
					addCommand(instrs, "push", "{" + reg + "}");
				}

				// borrow a register for #0xFFFFFFFF
				args.clear();
				args.push_back(reg);
				args.push_back(argLoc);
				string tempReg = Utils::borrowRegister(args);

				// push {tempReg}
				// mov tempReg, #0xFFFFFFFF
				addCommand(instrs, "push", "{" + tempReg + "}");
				addCommand(instrs, "mov", tempReg, "#0xFFFFFFFF");

				// eor reg, argLoc, tempReg
				addCommand(instrs, "eor", argLoc, tempReg);

				// pop {tempReg}
				addCommand(instrs, "pop", "{" + tempReg + "}");

			    if (!freeRegs.empty()) {
			    	retLoc = reg;
				} else {
					// str reg, [fp, #-4]
					addCommand(instrs, "str", reg, stackLoc);

					// pop {reg}
					addCommand(instrs, "pop", "{" + reg + "}");
					retLoc = stackLoc;
				}

				if (argOnStack) {
					addCommand(instrs, "pop", "{r4}");
					argLoc = argTempLoc;
				}

				treble_ptr_t ret(new treble_t(retLoc, instrs, freeRegs));
				return ret;
    		} else if (op == "~") {
				vector<string> args;
    			string stackLoc, reg;
    			bool onStack = false;
			    if (!freeRegs.empty()) {
			    	reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					onStack = true;
					func->increaseStackPointer(4);
					stackLoc = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(argLoc);
					reg = Utils::borrowRegister(argRegs);

					// push {reg}
					addCommand(instrs, "push", "{" + reg + "}");
				}

				// mvn reg, argLoc
				addCommand(instrs, "mvn", reg, argLoc);

				if (onStack) {
					// str reg, [fp, #-4]
					addCommand(instrs, "str", reg, stackLoc);

					// pop {reg}
					addCommand(instrs, "pop", "{" + reg + "}");
					reg = stackLoc;
				}

				if (argOnStack) {
					addCommand(instrs, "pop", "{r4}");
					argLoc = argTempLoc;
				}

				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
				return ret;
    		} else if (op == "+" || op == "-") {
				// unary +:
				// mov reg, argLoc
	        	// cmp reg, #0
				// neglt reg, reg

				// unary -:
				// mov reg, argLoc
				// cmp reg, #0
				// neggt reg, reg

				vector<string> args;
    			string reg, stackLoc;
    			bool onStack = false;
			    if (!freeRegs.empty()) {
			    	reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());

				} else {
					onStack = true;
					func->increaseStackPointer(4);
					stackLoc = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(argLoc);
					reg = Utils::borrowRegister(argRegs);

					// push {reg}
					addCommand(instrs, "push", "{" + reg + "}");
				}

				addCommand(instrs, "mov", reg, argLoc);
				addCommand(instrs, "cmp", reg, "#0");

				string negInstr = (op == "+") ? "neglt" : "neggt";

				addCommand(instrs, negInstr, reg, reg);

				if (onStack) {
					// str reg, [fp, #-4]
					addCommand(instrs, "str", reg, stackLoc);

					// pop {reg}
					addCommand(instrs, "pop", "{" + reg + "}");
					reg = stackLoc;
				}

				if (argOnStack) {
					addCommand(instrs, "pop", "{r4}");
					argLoc = argTempLoc;
				}

    			treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
    			return ret;
    	    }
    	} else if (children == 2) {
			// Binary operator

		    string op = Utils::createStringFromTree(root);

		    pANTLR3_BASE_TREE lhs = Utils::childByNum(root, 0);
			pANTLR3_BASE_TREE rhs = Utils::childByNum(root, 1);

			int lhsW = calculateWeight(lhs, freeRegs, st);
			int rhsW = calculateWeight(rhs, freeRegs, st);

			treble_ptr_t lhsEval, rhsEval;
			string lhsLoc, rhsLoc;
			if (lhsW < rhsW) {
				// Calculate lhs first sicne it uses fewer registers
				lhsEval = generateExpression(lhs, st, freeRegs, func);
				lhsLoc = lhsEval->get<0>();
				list<AssemCom> lhsInstrs = lhsEval->get<1>();
				freeRegs = lhsEval->get<2>();
				instrs.splice(instrs.end(), lhsInstrs);

				rhsEval = 
					generateExpression(rhs, st, freeRegs, func);
				rhsLoc = rhsEval->get<0>();
				list<AssemCom> rhsInstrs = rhsEval->get<1>();
				freeRegs = rhsEval->get<2>();
				instrs.splice(instrs.end(), rhsInstrs);
			} else {
				// Calculate rhs first
				rhsEval = generateExpression(rhs, st, freeRegs, func);
				rhsLoc = rhsEval->get<0>();
				list<AssemCom> rhsInstrs = rhsEval->get<1>();
				freeRegs = rhsEval->get<2>();
				instrs.splice(instrs.end(), rhsInstrs);

				lhsEval = 
					generateExpression(lhs, st, freeRegs, func);
				lhsLoc = lhsEval->get<0>();
				list<AssemCom> lhsInstrs = lhsEval->get<1>();
				freeRegs = lhsEval->get<2>();
				instrs.splice(instrs.end(), lhsInstrs);
			}

			vector<string> args;
			string lhsTempLoc;
			bool lhsOnStack = false;
			if (lhsLoc[0] != 'r') {
				lhsOnStack = true;
				string tempReg;
				if(!freeRegs.empty()) {
					tempReg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					tempReg = "r7";
				}
				
				addCommand(instrs, "push", "{" + tempReg + "}");
				addCommand(instrs, "ldr", tempReg, lhsLoc);

				if(lhsLoc[0] == '.') {
					addCommand(instrs, "ldr", tempReg, "[" + tempReg + "]");
				}

				lhsTempLoc = lhsLoc;
				lhsLoc = tempReg;
			}

			bool rhsOnStack = false;
			string rhsTempLoc;
			if (rhsLoc[0] != 'r') {
				rhsOnStack = true;
				string tempReg;
				if(!freeRegs.empty()) {
					tempReg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					tempReg = "r8";
				}

				addCommand(instrs, "push", "{" + tempReg + "}");
				addCommand(instrs, "ldr", tempReg, rhsLoc);

				if(rhsLoc[0] == '.') {
					addCommand(instrs, "ldr", tempReg, "[" + tempReg + "]");
				}

				rhsTempLoc = rhsLoc;
				rhsLoc = tempReg;
			}

			/*
				Binary Arithmetic operators
				NB: divison has a seperate case since it is more complex
			*/
			if (op == "||" || op == "|" || op == "&&" || op == "&" ||
				op == "^"  || op == "+" || op == "-"  || op == "*" ||
				op == "%") {
				vector<string> args;
    			string stackLoc, reg;
    			bool onStack = false;
				if (!freeRegs.empty()) {
			    	reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					onStack = true;
					func->increaseStackPointer(4);
					stackLoc = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(lhsLoc);
					argRegs.push_back(rhsLoc);
					reg = Utils::borrowRegister(argRegs);

					// push {reg}
					addCommand(instrs, "push", "{" + reg + "}");
				}

				if (op == "||" || op == "|") {
					// logical or bitwise or
					addCommand(instrs, "orr", reg, lhsLoc, rhsLoc);
				} else if (op == "&&" || op == "&") {
					// logical or bitwise and
					addCommand(instrs, "and", reg, lhsLoc, rhsLoc);
				} else if (op == "^") {
					// bitwise xor
					addCommand(instrs, "eor", reg, lhsLoc, rhsLoc);
				} else if (op == "+") {
					// addition
					addCommand(instrs, "add", reg, lhsLoc, rhsLoc);
				} else if (op == "-") {
					// subtraction
					addCommand(instrs, "sub", reg, lhsLoc, rhsLoc);
				} else if (op == "*") {
					// multiplication
					addCommand(instrs, "mul", reg, lhsLoc, rhsLoc);
				} else if (op == "%") {
					// modulo
					addCommand(instrs, "mov", reg, lhsLoc);

					Label l;
					instrs.push_back(AssemCom(l.getLabel() + 
										":", std::vector<string>()));

					addCommand(instrs, "sub", reg, reg, rhsLoc);
					addCommand(instrs, "cmp", reg, rhsLoc);
					addCommand(instrs, "bge", l.getLabel());
				}

				if (onStack) {
					// str reg, [fp, #-4]
					addCommand(instrs, "str", reg, stackLoc);

					// pop {reg}
					addCommand(instrs, "pop", "{" + reg + "}");
					reg = stackLoc;
				}

				if (rhsOnStack) {
					addCommand(instrs, "pop", "{" + rhsLoc + "}");
					rhsLoc = rhsTempLoc;
				}

				if (lhsOnStack) {
					addCommand(instrs, "pop", "{" + lhsLoc + "}");
					lhsLoc = lhsTempLoc;
				}

				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
				return ret;

			/*
				Divison, implemented by hand since armv6 does not have integer
				divison built in
			*/
			} else if (op == "/") {
				string res, reg, stackLocRes, stackLocReg;
				vector<string> args;
				bool resOnStack, regOnStack;
				if (!freeRegs.empty()) {
			    	res = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
					if (!freeRegs.empty()) {
						reg = freeRegs.front();
						freeRegs.erase(freeRegs.begin());
					} else {
						regOnStack = true;
						func->increaseStackPointer(4);
						stackLocReg = "[fp, #-" + 
							boost::lexical_cast<string>(func->getStackPointer()) + 
							"]";

						// borrow a regiser to replace stackLoc in any calculations
						vector<string> argRegs;
						argRegs.push_back(lhsLoc);
						argRegs.push_back(rhsLoc);
						reg = Utils::borrowRegister(argRegs);

						// push {reg}
						addCommand(instrs, "push", "{" + reg + "}");
					}

				} else {
					resOnStack = true;
					func->increaseStackPointer(4);
					stackLocRes = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(lhsLoc);
					argRegs.push_back(rhsLoc);
					res = Utils::borrowRegister(argRegs);

					// push {reg}
					addCommand(instrs, "push", "{" + res + "}");
				}

				addCommand(instrs, "eor", res, res, res);
				addCommand(instrs, "mov", reg, lhsLoc);

				Label l;
				instrs.push_back(AssemCom(l.getLabel() + 
										":", std::vector<string>()));

				addCommand(instrs, "sub", reg, reg, rhsLoc);
				addCommand(instrs, "add", res, res, "#1");
				addCommand(instrs, "cmp", reg, rhsLoc);
				addCommand(instrs, "bge", l.getLabel());

				freeRegs.push_back(reg);

				if (resOnStack && stackLocRes != "") {
					// str reg, [fp, #-4]
					addCommand(instrs, "str", res, stackLocRes);

					// pop {reg}
					addCommand(instrs, "pop", "{" + res + "}");
					res = stackLocRes;
				}

				if (regOnStack && stackLocReg != "") {
					// pop {reg}
					addCommand(instrs, "pop", "{" + reg + "}");
				}

				if (rhsOnStack) {
					addCommand(instrs, "pop", "{" + rhsLoc + "}");
					rhsLoc = rhsTempLoc;
				}

				if (lhsOnStack) {
					addCommand(instrs, "pop", "{" + lhsLoc + "}");
					lhsLoc = lhsTempLoc;
				}

				treble_ptr_t ret(new treble_t(res, instrs, freeRegs));
				return ret;

			/*
				Relational operators
				(logical AND and OR can be handled with their equivalent biwise
				operators and are therefore not included here)
			*/
			} else if (op == "==" || op == "!=" || op == ">" || op == "<" ||
					   op == ">=" || op == "<=") {
				vector<string> args;
    			string stackLoc, reg;
    			bool onStack = false;
				if (!freeRegs.empty()) {
			    	reg = freeRegs.front();
					freeRegs.erase(freeRegs.begin());
				} else {
					onStack = true;
					func->increaseStackPointer(4);
					stackLoc = "[fp, #-" + 
						boost::lexical_cast<string>(func->getStackPointer()) + 
						"]";

					// borrow a regiser to replace stackLoc in any calculations
					vector<string> argRegs;
					argRegs.push_back(lhsLoc);
					argRegs.push_back(rhsLoc);
					reg = Utils::borrowRegister(argRegs);

					// push {reg}
					addCommand(instrs, "push", "{" + reg + "}");

				}

				// eor reg, reg, reg
				// cmp lhsLoc, rhsLoc
				// mov<op> reg, #0xFFFFFFFF
				addCommand(instrs, "eor", reg, reg, reg);

				string tmpl = "";
				if (lhsLoc[0] == '.') {
					// global var
					pair<string, vector<string> > p = 
						Utils::tempForGlobal(lhsLoc, freeRegs, instrs);
					tmpl = p.first;
					freeRegs = p.second;
					lhsLoc = tmpl;
				}

				string tmpr = "";
				if (rhsLoc[0] == '.') {
					// global var
					pair<string, vector<string> > p = 
						Utils::tempForGlobal(rhsLoc, freeRegs, instrs);
					tmpr = p.first;
					freeRegs = p.second;
					rhsLoc = tmpr;
				}

				addCommand(instrs, "cmp", lhsLoc, rhsLoc);

				if (op == "==") {
					// Equal to
					addCommand(instrs, "moveq", reg, "#0xFFFFFFFF");
				} else if (op == "!=") {
					// Not equal to
					addCommand(instrs, "movne", reg, "#0xFFFFFFFF");
				} else if (op == ">") {
					// Greater than
					addCommand(instrs, "movgt", reg, "#0xFFFFFFFF");
				} else if (op == "<") {
					// Less than
					addCommand(instrs, "movlt", reg, "#0xFFFFFFFF");
				} else if (op == ">=") {
					// Greater than or equal to
					addCommand(instrs, "movge", reg, "#0xFFFFFFFF");
				} else if (op == "<=") {
					// Less than or equal to
					addCommand(instrs, "movle", reg, "#0xFFFFFFFF");
				}

				if (tmpl != "") {
					freeRegs.push_back(tmpl);
				}

				if (tmpr != "") {
					freeRegs.push_back(tmpr);
				}

				if (onStack) {
					// str reg, [fp, #-4]
					addCommand(instrs, "str", reg, stackLoc);

					// pop {reg}
					addCommand(instrs, "pop", "{" + reg + "}");
					reg = stackLoc;
				}

				if (rhsOnStack) {
					addCommand(instrs, "pop", "{" + rhsLoc + "}");
					rhsLoc = rhsTempLoc;
				}

				if (lhsOnStack) {
					addCommand(instrs, "pop", "{" + lhsLoc + "}");
					lhsLoc = lhsTempLoc;
				}

				treble_ptr_t ret(new treble_t(reg, instrs, freeRegs));
				return ret;
			}
    	}
    }
}

/*
	Calculate the value of a constant expression, can be used for allocating
		space for arrays etc
*/
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

    //Utils::printComErr("Could not evaluate expression at compile time.");
}

/*
	Returns the weight (in registers) of an expression tree, which can be used
		to allocate registers more efficiently in expression instruction
		generation
*/
int ExprGen::calculateWeight(pANTLR3_BASE_TREE tree, vector<string> freeRegs,
								boost::shared_ptr<SymbolTable> st) {
	string tok = Utils::createStringFromTree(tree);

	if (tok == "FUNC") {
		// Inline function call. We will need params.size() registers here
		pANTLR3_BASE_TREE cplTree = Utils::childByNum(tree, 1);
		return cplTree->getChildCount(cplTree);
	} else if (tok == "VAR") {
		// Variable. If it has already been assigned then we don't need any 
		// registers, otherwise we need one
		string varName = 
			Utils::createStringFromTree(Utils::childByNum(tree, 0));
	    boost::shared_ptr<Identifier> varIdent = 
	    	st->lookupCurrLevelAndEnclosingLevels(varName);

		if (varIdent->getBaseName() == "Type") {
			return 0;
		} else {
			boost::shared_ptr<Variable> var = 
				boost::shared_polymorphic_downcast<Variable>(varIdent);
			string loc = var->getAssLoc();

			if (loc == "" && !freeRegs.empty()) {
				return 1;
			} else {
				return 0;
			}
		}
	} else if (tok == "ARRMEMBER") {
		// Array member. If it's going on the stack then return 0, otherwise it
		// needs one register
		if (!freeRegs.empty()) {
			return 1;
		} else {
			return 0;
		}
	} else if (tok == "'") {
		// It's a character, same rules as ARRMEMBER
		if (!freeRegs.empty()) {
			return 1;
		} else {
			return 0;
		}
	} else if (tok == "STR") {
		// We never use registers for strings
		return 0;
	} else if (tok == "EXPR") {
		// There's a sub-expression, recurse
		return calculateWeight(Utils::childByNum(tree, 0), freeRegs, st);
	} else {
		if (!freeRegs.empty()) {
			int children = tree->getChildCount(tree);
			switch(children) {
				case 0:
					// Number base case, same as char case
					return 1;

				case 1:
					// Unary operator
					freeRegs.erase(freeRegs.begin());
					return 1 + 
					  calculateWeight(Utils::childByNum(tree, 0), freeRegs, st);

				case 2:
					// Binary operator
					freeRegs.erase(freeRegs.begin());
					pANTLR3_BASE_TREE lhs = Utils::childByNum(tree, 0);
					pANTLR3_BASE_TREE rhs = Utils::childByNum(tree, 1);

					int wLeft = calculateWeight(lhs, freeRegs, st);
					int wRight = calculateWeight(rhs, freeRegs, st);

					return 1 + wLeft + wRight;
			}
		}
	}
}

void ExprGen::addCommand(list<AssemCom>& instrs, string name, string arg0) {
	vector<string> args;
	args.push_back(arg0);
	instrs.push_back(AssemCom(name, args));
}

void ExprGen::addCommand(list<AssemCom>& instrs, string name, string arg0, 
							string arg1) {
	vector<string> args;
	args.push_back(arg0);
	args.push_back(arg1);
	instrs.push_back(AssemCom(name, args));
}

void ExprGen::addCommand(list<AssemCom>& instrs, string name, string arg0, 
							string arg1, string arg2) {
	vector<string> args;
	args.push_back(arg0);
	args.push_back(arg1);
	args.push_back(arg2);
	instrs.push_back(AssemCom(name, args));
}
