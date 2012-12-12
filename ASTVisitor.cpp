#include "ast/ASTNode.hpp"
#include "ast/IfBodyAST.hpp"
#include "ast/HeaderParamsAST.hpp"
#include "ast/ExprAST.hpp"
#include "ast/CallParamsAST.hpp"
#include "ASTVisitor.hpp"
#include "idents/Variable.hpp"
#include "Label.hpp"
#include "ExprGen.hpp"
#include "idents/Array.hpp"
#include <boost/tuple/tuple.hpp>
#include <boost/lexical_cast.hpp>

// This type is returned by ExprGen::generateExpression
typedef 
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > 
	treble_ptr_t;

// The ASTVisitor is passed the SymbolTable that holds the global variables and
// top level functions, so that it can deal with the special cases for global
// variables
ASTVisitor::ASTVisitor(boost::shared_ptr<SymbolTable> st) {
	_globalSt = st;

	vector<string> alignArg;
	alignArg.push_back("2");
	_endDefs.push_back(AssemCom(".align", alignArg));

	_init_io = true;
}

void ASTVisitor::visitProg(vector <boost::shared_ptr<ASTNode> > children) {
	vector<boost::shared_ptr<ASTNode> >::iterator i;
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this());
	}
}

void ASTVisitor::visitProcDec(string name, 
								boost::shared_ptr<HeaderParamsAST> params,
								vector <boost::shared_ptr<ASTNode> > children, 
								boost::shared_ptr<SymbolTable> st) {
	if (name == "hatta") {
		boost::shared_ptr<AssemFunc> func(new AssemFunc("main"));
		_functions.push_back(func);

		vector<int> callableKids;
		int i = 0;
		vector<boost::shared_ptr<ASTNode> >::iterator it;
		for (it = children.begin(); it != children.end(); ++it) {
			if ((*it)->getNodeName() == "FuncDec" 
					|| (*it)->getNodeName() == "ProcDec") {
				callableKids.push_back(i);
			} else {
				// visit function body
				(*it)->accept(shared_from_this(), func);
			}
			i++;
		}

		// moved nested functions outside of parent function
		vector<int>::iterator cIt;
		for (cIt = callableKids.begin(); cIt != callableKids.end(); ++cIt) {
			(*(children.begin() + (*cIt)))->accept(shared_from_this());				
		}

	} else {
		// If it's not the top level hatta then we can just do exactly the same
		// as for a ProcDec
		visitFuncDec(name, "", params, children, st);
	}
}

// same as above at the moment
void ASTVisitor::visitFuncDec(string name, string returnType,
								boost::shared_ptr<HeaderParamsAST> params,
								vector <boost::shared_ptr<ASTNode> > children,
								boost::shared_ptr<SymbolTable> st) {
	boost::shared_ptr<AssemFunc> func(new AssemFunc(name));
	_functions.push_back(func);
	vector< boost::shared_ptr<Param> > v = params->getParams();
    vector< boost::shared_ptr<Param> >::iterator param;

    // Reset the static list of registers containing constants
    ExprGen::_constRegs.clear();

    int i = 1;
    for (param = v.begin(); param != v.end(); ++param, ++i) {
       	boost::shared_ptr<Identifier> id = 
       		st->lookupCurrLevelOnly((*param)->getName());
        
        if ((*param)->getType()->getTypeName() == "Array") {
        	// Need to do a seperate downcast for Arrays
            boost::shared_ptr<Array> a 
            	= boost::shared_polymorphic_downcast<Array>(id);
            int rNum = ((i + 1) / 2) - 1;
			a->setAssLoc("r" + boost::lexical_cast<string>(rNum));
			func->removeReg("r" + boost::lexical_cast<string>(rNum));
		} else if ((*param)->getType()->getTypeName() == "Sentence") {
			// Do nothing since sentence is stored in a label already
        } else {
        	// Otherwise, set var location to correct register
            boost::shared_ptr<Variable> v 
            	= boost::shared_polymorphic_downcast<Variable>(id);
            int rNum = ((i + 1) / 2) - 1;
			v->setAssLoc("r" + boost::lexical_cast<string>(rNum));
			func->removeReg("r" + boost::lexical_cast<string>(rNum));
    	}
    	i++;
	}

	vector<int> callableKids;
	i = 0;
	vector<boost::shared_ptr<ASTNode> >::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		if ((*it)->getNodeName() == "FuncDec" 
				|| (*it)->getNodeName() == "ProcDec") {
			callableKids.push_back(i);
		} else {
			// function body
			(*it)->accept(shared_from_this(), func);
		}
		i++;
	}

	// moved nested functions outside of parent function
	vector<int>::iterator cIt;
	for (cIt = callableKids.begin(); cIt != callableKids.end(); ++cIt) {
		(*(children.begin() + (*cIt)))->accept(shared_from_this());				
	}
}

void ASTVisitor::visitVarDec(string typeName, string varName, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr<Identifier> varIdent = 
	  _globalSt->lookupCurrLevelOnly(varName);

	boost::shared_ptr<Identifier> localIdent = st->lookupCurrLevelOnly(varName);

	if (varIdent) {
		boost::shared_ptr<Variable> var = 
		  boost::shared_polymorphic_downcast<Variable>(varIdent);

		boost::shared_ptr<Variable> localVar = 
		  boost::shared_polymorphic_downcast<Variable>(localIdent);

		if (var->getAssLoc() == "" && localVar->getAssLoc() == "") {
			boost::shared_ptr<Type> varType = var->getTypeName();
			if (varType->getTypeName() == "Number") {
				// Allocate 4 bytes of space for our global number
				std::vector<string> comm;
				comm.push_back(varName);
				comm.push_back("4");
				_startDefs.push_back(AssemCom(".comm", comm));

				Label l;
				_startDefs.push_back(
					AssemCom(l.getLabel() + ":", std::vector<string>()));

				std::vector<string> word;
				word.push_back(varName);
				_startDefs.push_back(AssemCom(".word", word));

				var->setAssLoc(l.getLabel());
			} else if (varType->getTypeName() == "Letter") {
				// Only need 1 byte for a letter
				std::vector<string> comm;
				comm.push_back(varName);
				comm.push_back("1");
				_startDefs.push_back(AssemCom(".comm", comm));

				Label l;
				_startDefs.push_back(
					AssemCom(l.getLabel() + ":", std::vector<string>()));

				std::vector<string> word;
				word.push_back(varName);
				_startDefs.push_back(AssemCom(".word", word));

				var->setAssLoc(l.getLabel());
			} else {
				// String case - we don't need to do anything here since the 
				// string is stored in a label
			}
		}
	}
}

void ASTVisitor::visitInc(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg[0] != 'r') {
		// expression is stored on stack
		isStack = true;

		addCommand(func, "push", "{r0}");
		addCommand(func, "ldr", "r0", resultReg);

		resultReg = "r0";
	}

	addCommand(func, "add", resultReg, resultReg, "#1");

	if (isStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr,
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg[0] != 'r') {
		// expression is stored on stack
		isStack = true;
		addCommand(func, "push", "{r0}");
		addCommand(func, "ldr", "r0", resultReg);
		resultReg = "r0";
	}

	addCommand(func, "sub", resultReg, resultReg, "#1");

	if (isStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {

	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);

	func->setFreeRegs(res->get<2>());
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());

	bool isStack = false;
	string global = resultReg;
	string tmpReg;
	if (resultReg[0] != 'r' && resultReg[0] != '\"') {
		// expression is stored on stack
		isStack = true;

		vector<string> ignoreRegs;
		ignoreRegs.push_back("r0");
		ignoreRegs.push_back("r1");
		ignoreRegs.push_back(resultReg);
		// we need a register that won't be used later on in this function
		tmpReg = Utils::borrowRegister(ignoreRegs);

		addCommand(func, "push", "{" + tmpReg + "}");
		addCommand(func, "ldr", tmpReg, resultReg);

		resultReg = tmpReg;
	}

	Label strLbl;

	if (expr->getType()->getTypeName() == "Sentence") {
		// create label for sentence
		if (resultReg[0] != '.' && resultReg[0] == '\"') {
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
			vector<string> asciiArg;
			asciiArg.push_back(resultReg);
			_endDefs.push_back(AssemCom(".asciz", asciiArg));
		}
	} else {
		if (expr->getType()->getTypeName() == "Number") {
			// create label for integer format string
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
			vector<string> asciiArg;
			asciiArg.push_back("\"%i\"");
			_endDefs.push_back(AssemCom(".asciz", asciiArg));
		} else {
			// create label for character format string
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
			vector<string> asciiArg;
			asciiArg.push_back("\"%c\"");
			_endDefs.push_back(AssemCom(".asciz", asciiArg));
		}

		if (resultReg[0] != '.' && global[0] != '.') {
			// local variable or expression
			if (resultReg != "r1") {
				if (!func->regIsFree("r1")) {
					addCommand(func, "push", "{r1}");
				}

				addCommand(func, "mov", "r1", resultReg);
			}
		} else {
			//global varaible
			if (!func->regIsFree("r1")) {
				addCommand(func, "push", "{r1}");	
			}

			if(resultReg[0] == '.') {
				addCommand(func, "ldr", "r1", resultReg);
			} else {
				addCommand(func, "mov", "r1", resultReg);
			}

			addCommand(func, "ldr", "r1", "[r1]");
		}

	}

	if ((expr->getType()->getTypeName() == "Sentence" && resultReg[0] == '\"') 
		|| !(expr->getType()->getTypeName() == "Sentence")) {
		// load the contents of the label into the first argument
		addCommand(func, "push", "{r0}");
		addCommand(func, "ldr", "r0", "=" + strLbl.getLabel());
	}

	if (!func->regIsFree("r2")) {
		// we need to preserve r2, could be clobbered by printf
		addCommand(func, "push", "{r2}");
	}
	
	addCommand(func, "bl", "printf");

	if (!func->regIsFree("r2")) {
		// r2 is safe now
		addCommand(func, "pop", "{r2}");
	}


	// pop everything that we may have pushed earlier on
	if ((expr->getType()->getTypeName() == "Sentence" && resultReg[0] == '\"') 
		|| !(expr->getType()->getTypeName() == "Sentence")) {
		addCommand(func, "pop", "{r0}");
	}
	
	if (!func->regIsFree("r1")) {
		if (resultReg != "r1" && expr->getType()->getTypeName() != "Sentence") {
			addCommand(func, "pop", "{r1}");
		}
	}

	if (isStack) {
		addCommand(func, "pop", "{" + tmpReg + "}");
	}

}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);

	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg[0] != 'r') {
		// expr is on the stack
		isStack = true;
		addCommand(func, "ldr", "r0", resultReg);
		resultReg = "r0";
	}

	if (resultReg != "r0") {
		// as per the calling convention, move result into r0 if it's not there
		addCommand(func, "mov", "r0", resultReg);
	}

	addCommand(func, "ldmfd", "sp!", "{r4-r10, fp, pc}");
}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {
	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);

	func->setFreeRegs(res->get<2>());
	string resultReg = res->get<0>();

	bool isStack = false;
	string tmpReg;
	if (resultReg[0] == '[') {
		// expr is on the stack
		isStack = true;

		vector<string> ignoreRegs;
		ignoreRegs.push_back("r0");
		ignoreRegs.push_back("r1");
		// don't want to use r1 or r0 since they could be used later
		tmpReg = Utils::borrowRegister(ignoreRegs);

		addCommand(func, "push", "{" + tmpReg + "}");
		addCommand(func, "ldr", tmpReg, resultReg);

		resultReg = tmpReg;
	}

	if (expr->getType()->getTypeName() == "Sentence"){
		// allow some space on the stack for our new string
		func->increaseStackPointer(260);
		string sp = boost::lexical_cast<string>(func->getStackPointer());

		addCommand(func, "sub", "r0", "fp", "#"+sp);
		addCommand(func, "bl", "__isoc99_scanf");
		addCommand(func, "ldr", resultReg, "[fp, #-" + sp + "]");
	} else {
		// make some space on the stack for new value
		func->increaseStackPointer(8);
		string sp = boost::lexical_cast<string>(func->getStackPointer());
		
		Label strLbl;
		_endDefs.push_back(AssemCom(strLbl.getLabel() + ":", vector<string>()));
		vector<string> asciiArg;

		// put the format string in a label
		if (expr->getType()->getTypeName() == "Number") {	
			asciiArg.push_back("\"%i\"");
		} else if (expr->getType()->getTypeName() == "Letter") {
			asciiArg.push_back("\"%c\"");
		}
		_endDefs.push_back(AssemCom(".asciz", asciiArg));

		if(resultReg[0] != '.') {
			// global variable
			addCommand(func, "str", resultReg, "[fp, #-" + sp + "]");
			addCommand(func, "ldr", "r0", "=" + strLbl.getLabel());
			addCommand(func, "sub", "r1", "fp", "#" + sp);
			addCommand(func, "bl", "__isoc99_scanf");
			addCommand(func, "ldr", resultReg, "[fp, #-" + sp + "]");
		} else {
			// local variable
			addCommand(func, "ldr", "r0", "=" + strLbl.getLabel());
			addCommand(func, "ldr", "r1", resultReg);
			addCommand(func, "bl", "__isoc99_scanf");
		}
	}

	if (isStack) {
		addCommand(func, "pop", "{" + tmpReg + "}");
	}
}

void ASTVisitor::visitWhile(boost::shared_ptr<ExprAST> cond, 
			   			   	  vector <boost::shared_ptr<ASTNode> > children, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {

	// Do nothing if the loop body is empty
	if (!children.empty()) {
		Label loopLabel;
		addLabel(func, loopLabel.getLabel());

		vector<boost::shared_ptr<ASTNode> >::iterator i;
		for (i = children.begin(); i != children.end(); ++i) {
			(*i)->accept(shared_from_this(), func);
		}

		treble_ptr_t res = ExprGen::generateExpression(cond->getRoot(), st, 
													func->getFreeRegs(), func);
		string resultReg = res->get<0>();
		func->setFreeRegs(res->get<2>());
		func->addListBack(res->get<1>());

		bool onStack = false;
		if (resultReg[0] != 'r') {
			// result is stored on the stack
			onStack = true;
			addCommand(func, "push", "{r0}");
			addCommand(func, "ldr", "r0", resultReg);
			resultReg = "r0";
		}

		addCommand(func, "cmp", resultReg, "#0");
		// If the condition is not false (ie true) then loop
		addCommand(func, "bne", loopLabel.getLabel());

		if (onStack) {
			addCommand(func, "pop", "{r0}");
		}
	}
}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {

	// Check if both the if and else bodies are empty, if they are then do 
	// nothing
	if (!trueBody->isEmpty() || !falseBody->isEmpty()) {
		Label elseLabel;
		bool isStack = false;

		treble_ptr_t res = ExprGen::generateExpression(cond->getRoot(), st, 
													func->getFreeRegs(), func);
		string resultReg = res->get<0>();
		func->addListBack(res->get<1>());
		func->setFreeRegs(res->get<2>());

		if (resultReg[0] != 'r') {
			// result is stored on the stack
			isStack = true;
			addCommand(func, "push", "{r0}");
			addCommand(func, "ldr", "r0", resultReg);
			resultReg = "r0";
		}

		addCommand(func, "cmp", resultReg, "#0");

		if (isStack) {
			addCommand(func, "pop", "{r0}");
		}

		// jump if condition is false (boolean evaluating exprsesion == 0)
		addCommand(func, "beq", elseLabel.getLabel());

		trueBody->accept(shared_from_this(), func);

		Label endLabel;
		addCommand(func, "b", endLabel.getLabel());
		addLabel(func, elseLabel.getLabel());

		falseBody->accept(shared_from_this(), func);

		addLabel(func, endLabel.getLabel());
	}
}

void ASTVisitor::visitIf(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children, 
						   boost::shared_ptr<SymbolTable> st,
						   boost::shared_ptr<AssemFunc> func) {

	// Check if the loop body is empty. If it is, then we can ignore the whole
	// node (unless there is an else body, in which case we can use the negative
	// condition from the if to create the condition for the else).
	if (!trueBody->isEmpty() || children.size() > 1) {
		Label elseLabel;
		bool isStack = false;

		treble_ptr_t res = ExprGen::generateExpression(cond->getRoot(), st, 
		  											func->getFreeRegs(), func);

		string resultReg = res->get<0>();
		func->addListBack(res->get<1>());
		func->setFreeRegs(res->get<2>());

		if (resultReg[0] != 'r') {
			// result is stored on the stack
			isStack = true;
			addCommand(func, "push", "{r0}");
			addCommand(func, "ldr", "r0", resultReg);
			resultReg = "r0";
		}

		addCommand(func, "cmp", resultReg, "#0");

		if (isStack) {
			addCommand(func, "pop", "{r0}");
		}

		// jump if condition is false (boolean evaluating exprsesion == 0)
		addCommand(func, "beq", elseLabel.getLabel());

		trueBody->accept(shared_from_this(), func);

		if (children.size() == 1) {
			addLabel(func, elseLabel.getLabel());

		} else {
			Label endLabel;
			addCommand(func, "b", endLabel.getLabel());
			addLabel(func, elseLabel.getLabel());

			(*(children.begin() + 1))->accept(shared_from_this(), func);	

			addLabel(func, endLabel.getLabel());
		}
	}
}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr<Identifier> varIdent 
	  = st->lookupCurrLevelAndEnclosingLevels(varName);
	boost::shared_ptr<Variable> var 
	  = boost::shared_polymorphic_downcast<Variable>(varIdent);

	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);

	string loc = var->getAssLoc();

	if (var->getTypeName()->getTypeName() == "Sentence") {
		// Create label with new value
		vector<string> asciiArg;
		asciiArg.push_back(res->get<0>());
		Label strLbl;
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
		var->setAssLoc(strLbl.getLabel());
		_endDefs.push_back(AssemCom(".asciz", asciiArg));
	} else {
		string rhs = res->get<0>();
		func->setFreeRegs(res->get<2>());
		func->addListBack(res->get<1>());

		bool rhsOnStack = false;
		string tmpReg;
		if (rhs[0] != 'r') {
			// it's on the stack
			rhsOnStack = true;

			// r0 might be pushed later, so let's not use that now
			vector<string> ignoreRegs;
			ignoreRegs.push_back("r0");

			tmpReg = Utils::borrowRegister(ignoreRegs);
			addCommand(func, "push", "{" + tmpReg + "}");
			addCommand(func, "ldr", tmpReg, rhs);
			rhs = tmpReg;
		}

		if (loc == "") {
			// variable hasn't been assigned to before, so we need to find it a 
			// location
			var->setVal(ExprGen::evaluateExpression(expr->getRoot(), st));
			if (func->getFreeRegs().empty()) {
				func->increaseStackPointer(4);
				string stackLoc = "[fp,#-" + 
					boost::lexical_cast<string>(func->getStackPointer()) + "]";
				var->setAssLoc(stackLoc);
			} else {
				var->setAssLoc(rhs);
				func->removeReg(var->getAssLoc());
			}
		} else if (loc[0] == '.') {
			// global variable assignment
			string wordLoc;
			vector<string> ldArgs;
			bool onStack = false;

			if (func->getFreeRegs().empty()) {
				onStack = true;
				func->increaseStackPointer(4);
				string stackLoc = "[fp,#-" + 
					boost::lexical_cast<string>(func->getStackPointer()) + "]";
				// don't need r0 later on so this is fine
				wordLoc = "r0";
				addCommand(func, "push", "{" + wordLoc + "}");
			} else {
				wordLoc = func->getFreeRegs().front();
			}

			addCommand(func, "ldr", wordLoc, loc);

			if (var->getTypeName()->getTypeName() == "Letter") {
				addCommand(func, "strb", rhs, "[" + wordLoc + "]");
			} else {
				addCommand(func, "str", rhs, "[" + wordLoc + "]");
			}

			// pop anything we might have pushed further up
			if (onStack) {
				addCommand(func, "pop", "{" + wordLoc + "}");
			}

			if (rhsOnStack) {
				addCommand(func, "pop", "{" + tmpReg + "}");
			}
		} else {
			// The simple case, local variable in register
			addCommand(func, "mov", var->getAssLoc(), rhs);

			if (rhsOnStack) {
				addCommand(func, "pop", "{" + tmpReg + "}");
			}
		}
	}
	
}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st) {
	// Global inline assignments
	// We just add the stuff to assign it to _globalInlines, which is added to 
	// the start of main at the end of processing the AST

	string regs[] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};

	vector<string> freeRegs =
		vector<string>(regs, regs + sizeof(regs) / sizeof(string));

	boost::shared_ptr<Identifier> varIdent 
	  = st->lookupCurrLevelAndEnclosingLevels(varName);
	boost::shared_ptr<Variable> var 
	  = boost::shared_polymorphic_downcast<Variable>(varIdent);

	if (var->getTypeName()->getTypeName() != "Sentence") {
		string loc = var->getAssLoc();
		vector<string> ldArgs;
		ldArgs.push_back("r0");
		ldArgs.push_back(loc);
		_globalInlines.push_back(AssemCom("ldr", ldArgs));
	}

	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
									freeRegs, boost::shared_ptr<AssemFunc>());
	if (var->getTypeName()->getTypeName() == "Sentence") {
		vector<string> asciiArg;
		asciiArg.push_back(res->get<0>());
		Label strLbl;
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
		var->setAssLoc(res->get<0>());
		_endDefs.push_back(AssemCom(".asciz", asciiArg));
	} else {
		string rhs = res->get<0>();
		_globalInlines.splice(_globalInlines.end(), res->get<1>());

		// global variable assignment
		// r0 is fine since it's always going to be the first thing
		string wordLoc = "r0"; 

		if (var->getTypeName()->getTypeName() == "Letter") {
			vector<string> strbArgs;
			strbArgs.push_back(rhs);
			strbArgs.push_back("[" + wordLoc + "]");
			_globalInlines.push_back(AssemCom("strb", strbArgs));
		} else {
			// must be a global number
			var->setVal(ExprGen::evaluateExpression(expr->getRoot(), st));
			vector<string> strbArgs;
			strbArgs.push_back(rhs);
			strbArgs.push_back("[" + wordLoc + "]");
			_globalInlines.push_back(AssemCom("str", strbArgs));
		}
	}

}

void ASTVisitor::visitFuncCall(string name,
						    	 boost::shared_ptr<CallParamsAST> params, 
							     boost::shared_ptr<SymbolTable> st,
							     boost::shared_ptr<AssemFunc> func) {

	// Sort the parameters into the correct registers as per the ARM calling
	// convention, then branch

	vector<boost::shared_ptr< ExprAST> > exprs = params->getParamExprs();
  			
  	int maxpush = 3;
	if (!func->getFreeRegs().empty()) {
		maxpush = min(func->getFreeRegs().front()[1] - 48, 3);
	}
	
	vector<boost::shared_ptr< ExprAST> >::iterator it;
	int i = 0;
	for (it = exprs.begin(); it != exprs.end(); ++it) {
		pANTLR3_BASE_TREE cp = (*it)->getRoot();

		treble_ptr_t genParam
	  	  = ExprGen::generateExpression(cp, st, func->getFreeRegs(), func);

	  	string paramLoc = genParam->get<0>();
		func->addListBack(genParam->get<1>());
		func->setFreeRegs(genParam->get<2>());

		if ((*it)->getType()->getTypeName() == "Sentence") {
			vector<string> asciiArg;
			asciiArg.push_back(paramLoc);
			Label strLbl;
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
			_endDefs.push_back(AssemCom(".asciz", asciiArg));

			//Parameter needs to be moved into correct register
			addCommand(func, "ldr", "r" + boost::lexical_cast<string>(i), 
						 "=" + strLbl.getLabel());
		} else if ((*it)->getType()->getTypeName() == "Array") {
			// Set up the pass by reference
			string arrLoc = 
				paramLoc.substr(paramLoc.find("-") + 1, paramLoc.size() - 8);

			addCommand(func, "push", "{r" + boost::lexical_cast<string>(i) + 
								"}");
			addCommand(func, "sub", "r" + boost::lexical_cast<string>(i), "fp", 
								"#" + arrLoc);
		} else if (i < 4) {
			if (paramLoc[0] != 'r') {
				string tempReg = Utils::borrowRegister(vector<string>());
				addCommand(func, "ldr", tempReg, paramLoc);

				paramLoc = tempReg;
			}

			if (paramLoc != "r" + boost::lexical_cast<string>(i)) {
  				addCommand(func, "mov", 
  								 "r" + boost::lexical_cast<string>(i),
  						 		 paramLoc);
  			}

			for (int j = 1; j < maxpush; ++j) {
				addCommand(func, "push", "{r" + boost::lexical_cast<string>(j) +
							 "}");
			}
	  	} else {
	  		// Push any other params
			if (paramLoc[0] == 'r') {
				addCommand(func, "push", "{" + paramLoc + "}");
			} else if (func->getFreeRegs().empty()) {
				addCommand(func, "push", "{r0}");
				addCommand(func, "ldr", "r0", paramLoc);

				addCommand(func, "pop", "{r0}");
			} else {
				string reg = func->getFreeRegs().front();
				addCommand(func, "push", "{" + reg + "}");
				addCommand(func, "mov", reg, paramLoc);
			}
	  	}
	  	i++;
	}

	addCommand(func, "bl", name);

	for (int j = maxpush - 1; j >= 1; --j) {
		addCommand(func, "pop", "{r" + boost::lexical_cast<string>(j) + "}");
	}
}

void ASTVisitor::visitArrayAssign(string name,
                  					boost::shared_ptr<ExprAST> index,
                  					boost::shared_ptr<ExprAST> value, 
							        boost::shared_ptr<SymbolTable> st,
							        boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr<Identifier> arrIdent = 
	  st->lookupCurrLevelAndEnclosingLevels(name);
	boost::shared_ptr<Array> arr =  
	  boost::shared_polymorphic_downcast<Array>(arrIdent);
	
	int indexVal = ExprGen::evaluateExpression(index->getRoot(), st);

	string arrayLoc = arr->getAssLoc();

	treble_ptr_t val = ExprGen::generateExpression(value->getRoot(), st, 
													 func->getFreeRegs(), func);
	string valReg = val->get<0>();
	func->addListBack(val->get<1>());
	func->setFreeRegs(val->get<2>());

	bool valOnStack = false;

	if (valReg[0] != 'r') {
		// it's on the stack
		valOnStack = true;
		addCommand(func, "push", "{r5}");
		addCommand(func, "ldr", "r5", valReg);
		valReg = "r5";
	}

	if (arrayLoc[0] == '[') {
		// The array is local
		int arrayStartIndex = atoi(arrayLoc.substr(arrayLoc.find("-")).c_str());
		int indexLoc = arrayStartIndex + (indexVal * 4);
		addCommand(func, "str", valReg, 
						"[fp, #" + boost::lexical_cast<string>(indexLoc) + "]");
	} else if (arrayLoc[0] == '.') {
		// array is global
		indexVal *= 4;
		string tempArrReg = 
			Utils::borrowRegister(std::vector<string>(1, valReg));
		addCommand(func, "ldr", tempArrReg, arrayLoc);
		addCommand(func, "str", valReg, "[" + tempArrReg + ", #" + 
								boost::lexical_cast<string>(indexVal) + "]");
		arrayLoc = tempArrReg;
	} else {
		// reference case
		indexVal *= 4;
		addCommand(func, "str", valReg, 
		  "[" + arrayLoc + ", #" + boost::lexical_cast<string>(indexVal) + "]");
	}

	if (valOnStack) {
		addCommand(func, "pop", "{r5}");
	}
}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
								 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st,
							     boost::shared_ptr<AssemFunc> func) {

	// local array declaration, just make some space on the stack for it
	boost::shared_ptr<Identifier> arrIdent = 
		st->lookupCurrLevelAndEnclosingLevels(name);
	boost::shared_ptr<Array> arr =  
			boost::shared_polymorphic_downcast<Array>(arrIdent);

	treble_ptr_t res = ExprGen::generateExpression(length->getRoot(), st, 
													 func->getFreeRegs(), func);

	string lengthReg = res->get<0>();
	func->addListBack(res->get<1>());

	int len = (ExprGen::evaluateExpression(length->getRoot(), st) + 1) * 4;

	func->increaseStackPointer(len);

	arr->setAssLoc("[fp, #-" + boost::lexical_cast<string>(len) + "]");

}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
								 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st) {

	// global array declaration, .comm some space at the top of the file for it
	boost::shared_ptr<Identifier> arrIdent = 
										_globalSt->lookupCurrLevelOnly(name);
	boost::shared_ptr<Array> arr =  
		boost::shared_polymorphic_downcast<Array>(arrIdent);
	std::vector<string> comm;
	comm.push_back(name);
	int len = ExprGen::evaluateExpression(length->getRoot(), st);
	
	if (type->getTypeName() == "Number") {
		len *= 4;
	}

	string convert = boost::lexical_cast<string>(len);      

	comm.push_back(convert);
	
	_startDefs.push_back(AssemCom(".comm", comm));

	Label l;
	_startDefs.push_back(AssemCom(l.getLabel() + ":", std::vector<string>()));
	std::vector<string> word;
	word.push_back(name);
	_startDefs.push_back(AssemCom(".word", word));
	arr->setAssLoc(l.getLabel());
		
}

/*
	The rest of the visit methods are for nodes used in our extension
*/
void ASTVisitor::visitMakeIn(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	
	if(_init_io) {
		_init_io = false;
		addCommand(func, "bl", "init_io");
	}

	treble_ptr_t res = ExprGen::generateExpression(expr->getRoot(), st, 
													 func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg != "r0") {
		// it's not where we want it.
		isStack = true;

		addCommand(func, "push", "{r0}");
		string command = resultReg[0] != 'r' ? "ldr" : "mov";
		addCommand(func, command, "r0", resultReg);

		resultReg = "r0";
	}

	addCommand(func, "bl", "make_input");

	if (isStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitMakeOut(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	
	if(_init_io) {
		_init_io = false;
		addCommand(func, "bl", "init_io");
	}

	treble_ptr_t res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg != "r0") {
		// it's not where we want it.
		isStack = true;

		addCommand(func, "push", "{r0}");
		string command = resultReg[0] != 'r' ? "ldr" : "mov";
		addCommand(func, command, "r0", resultReg);

		resultReg = "r0";
	}

	addCommand(func, "bl", "make_output");

	if (isStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitPause(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	
	int pin = ExprGen::evaluateExpression(expr->getRoot(), st);

	string number = boost::lexical_cast<string>(pin);

	//addCommand(func, "push", "{r0, r1}");
	addCommand(func, "ldr", "r0", "=" + number);
	addCommand(func, "mov", "r1", "#1000");
	addCommand(func, "mul", "r0", "r0", "r1");
	addCommand(func, "bl", "usleep");
	//addCommand(func, "pop", "{r0, r1}");
    addCommand(func, ".ltorg", "");

}

void ASTVisitor::visitReadIn(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	treble_ptr_t res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg != "r0") {
		// it's not where we want it.
		isStack = true;

		addCommand(func, "push", "{r0}");
		string command = resultReg[0] != 'r' ? "ldr" : "mov";
		addCommand(func, command, "r0", resultReg);

		resultReg = "r0";
	}
	
	addCommand(func, "mul", resultReg, resultReg, "#1000");

	addCommand(func, "bl", "usleep");

	if (isStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitSetHigh(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	
	int pin = ExprGen::evaluateExpression(expr->getRoot(), st);
	string number = boost::lexical_cast<string>(pin);

	//addCommand(func, "push", "{r0}");
	addCommand(func, "mov", "r0", "#" + number);
	addCommand(func, "bl", "set_high");
	//addCommand(func, "pop", "{r0}");
}

void ASTVisitor::visitSetLow(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	int pin = ExprGen::evaluateExpression(expr->getRoot(), st);
	string number = boost::lexical_cast<string>(pin);

	//addCommand(func, "push", "{r0}");
	addCommand(func, "mov", "r0", "#" + number);
	addCommand(func, "bl", "set_low");
	//addCommand(func, "pop", "{r0}");
}

void ASTVisitor::addLabel(boost::shared_ptr<AssemFunc> f, string label) {
	f->addBack(label + ":", std::vector<string>());
}
   
void ASTVisitor::addCommand(boost::shared_ptr<AssemFunc> f, string comm, 
							  string arg0) {
	vector<string> args;
	args.push_back(arg0);
	f->addBack(comm, args);
}

void ASTVisitor::addCommand(boost::shared_ptr<AssemFunc> f, string comm, 
							  string arg0, string arg1) {
	vector<string> args;
	args.push_back(arg0);
	args.push_back(arg1);
	f->addBack(comm, args);
}
    
void ASTVisitor::addCommand(boost::shared_ptr<AssemFunc> f, string comm, 
							  string arg0, string arg1, string arg2) {
	vector<string> args;
	args.push_back(arg0);
	args.push_back(arg1);
	args.push_back(arg2);
	f->addBack(comm, args);
}

vector<boost::shared_ptr<AssemFunc> > ASTVisitor::getFunctions() {
	return _functions;
}

list<AssemCom> ASTVisitor::getStartDefs() {
	return _startDefs;
}

list<AssemCom> ASTVisitor::getGlobalInlines() {
	return _globalInlines;
}

list<AssemCom>& ASTVisitor::getEndDefs() {
	return _endDefs;
}
