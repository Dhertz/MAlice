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

ASTVisitor::ASTVisitor(boost::shared_ptr<SymbolTable> st) {
	_globalSt = st;

	vector<string> alignArg;
	alignArg.push_back("2");
	_endDefs.push_back(AssemCom(".align", alignArg));							// .align 2
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
		vector<boost::shared_ptr<ASTNode> >::iterator it;						// function body
		for (it = children.begin(); it != children.end(); ++it) {
			if ((*it)->getNodeName() == "FuncDec" 
					|| (*it)->getNodeName() == "ProcDec") {
				callableKids.push_back(i);
			} else {
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

    int i = 1;
    for (param = v.begin(); param != v.end(); ++param, ++i) {
       	boost::shared_ptr<Identifier> id = 
       		st->lookupCurrLevelOnly((*param)->getName());
        
        if ((*param)->getType()->getTypeName() == "Array") {
            boost::shared_ptr<Array> a 
            	= boost::shared_polymorphic_downcast<Array>(id);
            int rNum = ((i + 1) / 2) - 1;
			a->setAssLoc("r" + boost::lexical_cast<string>(rNum));
			func->removeReg("r" + boost::lexical_cast<string>(rNum));
		} else if ((*param)->getType()->getTypeName() == "Sentence") {

        } else {
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
	// function body
	vector<boost::shared_ptr<ASTNode> >::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		if ((*it)->getNodeName() == "FuncDec" 
				|| (*it)->getNodeName() == "ProcDec") {
			callableKids.push_back(i);
		} else {
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
				// String case
			}
		}
	}
}

void ASTVisitor::visitInc(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg[0] != 'r') {
		// it's on the stack
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
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg[0] != 'r') {
		// it's on the stack
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

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);

	func->setFreeRegs(res->get<2>());
	string resultReg = res->get<0>();

	bool isStack = false;
	string global = resultReg;
	string tmpReg;
	if (resultReg[0] != 'r' && resultReg[0] != '\"') {
		// it's on the stack
		isStack = true;

		vector<string> ignoreRegs;
		ignoreRegs.push_back("r0");
		ignoreRegs.push_back("r1");
		tmpReg = Utils::borrowRegister(ignoreRegs);

		addCommand(func, "push", "{" + tmpReg + "}");
		addCommand(func, "ldr", tmpReg, resultReg);

		resultReg = tmpReg;
	}

	Label strLbl;

	if (expr->getType()->getTypeName() == "Sentence") {
		if (resultReg[0] != '.' && resultReg[0] == '\"') {
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));		// strLbl:
			vector<string> asciiArg;
			asciiArg.push_back(resultReg);
			_endDefs.push_back(AssemCom(".asciz", asciiArg));
		}
	} else {
		if (expr->getType()->getTypeName() == "Number") {
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));		// strLbl:
			vector<string> asciiArg;
			asciiArg.push_back("\"%i\"");
			_endDefs.push_back(AssemCom(".asciz", asciiArg));
		} else {
			_endDefs.push_back(
				AssemCom(strLbl.getLabel() + ":", std::vector<string>()));		// strLbl:
			vector<string> asciiArg;
			asciiArg.push_back("\"%c\"");
			_endDefs.push_back(AssemCom(".asciz", asciiArg));
		}

		if (resultReg[0] != '.' && global[0] != '.') {
			// local variable or expression
			func->addListBack(res->get<1>());									// expr instrs

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
			}

			addCommand(func, "ldr", "r1", "[r1]");
		}

	}

	if (!func->regIsFree("r0") && resultReg[0] == '\"') {
		addCommand(func, "push", "{r0}");
	}

	if ((expr->getType()->getTypeName() == "Sentence" && resultReg[0] == '\"') 
		|| !(expr->getType()->getTypeName() == "Sentence")) {
		addCommand(func, "ldr", "r0", "=" + strLbl.getLabel());
	}

	addCommand(func, "bl", "printf");

	if (!func->regIsFree("r0") && resultReg[0] == '\"') {
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
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);

	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	bool isStack = false;

	if (resultReg[0] != 'r') {
		// it's on the stack
		isStack = true;
		addCommand(func, "push", "{r0}");
		addCommand(func, "ldr", "r0", resultReg);
		resultReg = "r0";
	}

	if (resultReg != "r0") {
		addCommand(func, "mov", "r0", resultReg);
	}

	addCommand(func, "bx", "lr");

	if (isStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);

	func->setFreeRegs(res->get<2>());
	string resultReg = res->get<0>();

	bool isStack = false;
	string tmpReg;
	if (resultReg[0] != 'r' && resultReg[0] != '\"') {
		// it's on the stack
		isStack = true;

		vector<string> ignoreRegs;
		ignoreRegs.push_back("r0");
		ignoreRegs.push_back("r1");
		tmpReg = Utils::borrowRegister(ignoreRegs);

		addCommand(func, "push", "{" + tmpReg + "}");
		addCommand(func, "ldr", tmpReg, resultReg);

		resultReg = tmpReg;
	}

	if (expr->getType()->getTypeName() == "Sentence"){
		
		func->increaseStackPointer(260);
		string sp = boost::lexical_cast<string>(func->getStackPointer());

		addCommand(func, "sub", "r0", "fp", "#"+sp);
		addCommand(func, "bl", "__isoc99_scanf");
		addCommand(func, "ldr", resultReg, "[fp, #-" + sp + "]");
	} else {
		func->increaseStackPointer(8);
		string sp = boost::lexical_cast<string>(func->getStackPointer());

		string temp = "";
		if (resultReg[0] == '.') {
			// it's a global
			temp = resultReg;
			list<AssemCom> tempList;
			pair<string, vector<string> > p
			  = Utils::tempForGlobal(resultReg, func->getFreeRegs(), tempList);
			resultReg = p.first;
			func->setFreeRegs(p.second);
			func->addListBack(tempList);
		}

		Label strLbl;
		addCommand(func, "str", resultReg, "[fp, #-" + sp + "]");
		_endDefs.push_back(AssemCom(strLbl.getLabel() + ":", vector<string>()));
		vector<string> asciiArg;

		if (expr->getType()->getTypeName() == "Number") {	
			asciiArg.push_back("\"%i\"");										// .asciz "%i"
		} else if (expr->getType()->getTypeName() == "Letter") {
			asciiArg.push_back("\"%c\"");										// .asciz "%c"
		}
		
		_endDefs.push_back(AssemCom(".asciz", asciiArg));

		addCommand(func, "ldr", "r0", "=" + strLbl.getLabel());
		addCommand(func, "sub", "r1", "fp", "#" + sp);
		addCommand(func, "bl", "__isoc99_scanf");
		addCommand(func, "ldr", resultReg, "[fp, #-" + sp + "]");

		if (temp != "") {
			addCommand(func, "str", resultReg, temp);
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
	Label loopLabel;
	addLabel(func, loopLabel.getLabel());										// loop:

	vector<boost::shared_ptr<ASTNode> >::iterator i;							// loop body
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this(), func);
	}

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(cond->getRoot(), st, func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->setFreeRegs(res->get<2>());
	func->addListBack(res->get<1>());											// expr instrs

	bool onStack = false;
	if (resultReg[0] != 'r') {
		// result is stored on the stack
		onStack = true;
		addCommand(func, "push", "{r0}");
		addCommand(func, "ldr", "r0", resultReg);
		resultReg = "r0";
	}

	addCommand(func, "cmp", resultReg, "#0");
	addCommand(func, "beq", loopLabel.getLabel());

	if (onStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	Label elseLabel;
	bool isStack = false;

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(cond->getRoot(), st, func->getFreeRegs(), func);
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

	addCommand(func, "beq", elseLabel.getLabel());

	trueBody->accept(shared_from_this(), func);									// if body

	Label endLabel;
	addCommand(func, "b", endLabel.getLabel());
	addLabel(func, elseLabel.getLabel());										// else:

	falseBody->accept(shared_from_this(), func);								// else body

	addLabel(func, endLabel.getLabel());										// end:
}

void ASTVisitor::visitIf(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children, 
						   boost::shared_ptr<SymbolTable> st,
						   boost::shared_ptr<AssemFunc> func) {

	Label elseLabel;
	bool isStack = false;

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(cond->getRoot(), st, func->getFreeRegs(), func);

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

	addCommand(func, "beq", elseLabel.getLabel());

	trueBody->accept(shared_from_this(), func);									// if body

	if (children.size() == 1) {
		addLabel(func, elseLabel.getLabel());									// end:

	} else {
		Label endLabel;
		addCommand(func, "b", endLabel.getLabel());
		addLabel(func, elseLabel.getLabel());									// else:

		(*(children.begin() + 1))->accept(shared_from_this(), func);	

		addLabel(func, endLabel.getLabel());									// end:
	}
}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr<Identifier> varIdent 
	  = st->lookupCurrLevelAndEnclosingLevels(varName);
	boost::shared_ptr<Variable> var 
	  = boost::shared_polymorphic_downcast<Variable>(varIdent);

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res 
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs(), func);

	string loc = var->getAssLoc();

	if (var->getTypeName()->getTypeName() == "Sentence") {
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
		func->addListBack(res->get<1>());										// expr instrs

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

			if (onStack) {
				addCommand(func, "pop", "{" + wordLoc + "}");
			}

			if (rhsOnStack) {
				addCommand(func, "pop", "{" + tmpReg + "}");
			}
		} else {
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
		_globalInlines.push_back(AssemCom("ldr", ldArgs));						// ldr wordLoc loc
	}

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res 
	  = ExprGen::generateExpression(expr->getRoot(), st, freeRegs, boost::shared_ptr<AssemFunc>());
	if (var->getTypeName()->getTypeName() == "Sentence") {
		vector<string> asciiArg;
		asciiArg.push_back(res->get<0>());
		Label strLbl;
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":", std::vector<string>()));
		var->setAssLoc(strLbl.getLabel());
		_endDefs.push_back(AssemCom(".asciz", asciiArg));
	} else {
		string rhs = res->get<0>();
		_globalInlines.splice(_globalInlines.end(), res->get<1>());				// expr instrs

		// global variable assignment
		// fine since it's always going to be the first thing
		string wordLoc = "r0"; 
		

		if (var->getTypeName()->getTypeName() == "Letter") {
			vector<string> strbArgs;
			strbArgs.push_back(rhs);
			strbArgs.push_back("[" + wordLoc + "]");
			_globalInlines.push_back(AssemCom("strb", strbArgs));				// strb rhs [wordloc]
		} else {
			// must be a global number
			var->setVal(ExprGen::evaluateExpression(expr->getRoot(), st));
			vector<string> strbArgs;
			strbArgs.push_back(rhs);
			strbArgs.push_back("[" + wordLoc + "]");
			_globalInlines.push_back(AssemCom("str", strbArgs));				// str rhs [wordloc]
		}
	}

}

void ASTVisitor::visitFuncCall(string name,
						    	 boost::shared_ptr<CallParamsAST> params, 
							     boost::shared_ptr<SymbolTable> st,
							     boost::shared_ptr<AssemFunc> func) {

	vector<boost::shared_ptr< ExprAST> > exprs = params->getParamExprs();

	vector<boost::shared_ptr< ExprAST> >::iterator it;
	int i = 0;

	for (it = exprs.begin(); it != exprs.end(); ++it) {
		pANTLR3_BASE_TREE cp = (*it)->getRoot();

		boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > genParam
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
	  	} else {
	  		// Push any other params
			if (paramLoc[0] == 'r') {
				addCommand(func, "push", "{" + paramLoc + "}");
			} else if (func->getFreeRegs().empty()) {
				addCommand(func, "push", "{r0}");
				addCommand(func, "ldr", "r0", paramLoc);// check ldr here - Owen

				// WHy is this here? - Owen
				// addCommand(func, "push", "{r0}");

				addCommand(func, "pop", "{r0}");
			} else {
				string reg = func->getFreeRegs().front();
				addCommand(func, "mov", reg, paramLoc);
				addCommand(func, "push", "{" + reg + "}");
			}
	  	}
	  	i++;
	}

	addCommand(func, "bl", name);
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
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > ind
	   = ExprGen::generateExpression(index->getRoot(), st, func->getFreeRegs(), func);
	
	string resultReg = ind->get<0>();
	func->addListBack(ind->get<1>());
	func->setFreeRegs(ind->get<2>());
	bool resOnStack = false;

	if (resultReg[0] != 'r') {
		// it's on the stack
		resOnStack = true;
		addCommand(func, "push", "{r0}");
		addCommand(func, "ldr", "r0", resultReg);
		resultReg = "r0";
	}

	//make size bigger for integers
	if (arr->getElemType()->getTypeName() == "Number") {
		addCommand(func, "mov", resultReg, resultReg, "LSL #2");
	}
	
	string reg = arr->getAssLoc();

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > val
	  		= ExprGen::generateExpression(value->getRoot(), st, func->getFreeRegs(), func);
	string valReg = val->get<0>();
	func->addListBack(val->get<1>());
	func->setFreeRegs(val->get<2>());

	bool valOnStack = false;

	if (valReg[0] != 'r') {
		// it's on the stack
		valOnStack = true;
		addCommand(func, "push", "{r1}");
		addCommand(func, "ldr", "r1", valReg);
		valReg = "r1";
	}

	string tempArrReg = "";
	if (reg[0] == '.') {
		// array is global
		tempArrReg = func->getFreeRegs().front();
		addCommand(func, "ldr", tempArrReg, reg);
		reg = tempArrReg;
	}

	addCommand(func, "str", valReg, "[" + reg + ", " + resultReg + "]");

	if (valOnStack) {
		addCommand(func, "pop", "{r1}");
	}

	if (resOnStack) {
		addCommand(func, "pop", "{r0}");
	}
}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
								 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st,
							     boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr<Identifier> arrIdent = 
		st->lookupCurrLevelAndEnclosingLevels(name);
	boost::shared_ptr<Array> arr =  
			boost::shared_polymorphic_downcast<Array>(arrIdent);
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
  			= ExprGen::generateExpression(length->getRoot(), st, func->getFreeRegs(), func);
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());

	bool onStack = false;
	string tempReg;
	if (type->getTypeName() == "Number") {
		if (resultReg[0] == 'r') {
			//make it bigger for integers
			addCommand(func, "mov", resultReg, resultReg, "LSL #2");
		} else {
			tempReg = func->getFreeRegs().front();
			addCommand(func, "ldr", tempReg, resultReg);
			addCommand(func, "mov", tempReg, tempReg, "LSL #2");
			addCommand(func, "str", tempReg, resultReg);
		}
	}

	string reg;
	if (func->getFreeRegs().empty()) {
		onStack = true;
		addCommand(func, "push", "{r0}");
		reg = "r0";
	} else {
		reg = func->getFreeRegs().front();
		func->removeReg(reg);
		arr->setAssLoc(reg);
	}

	if (res->get<1>().size() < 1) {
		// length is not known, let's look at its location
		func->increaseStackPointerByReg(resultReg);
		if (resultReg[0] == 'r') {
			addCommand(func, "sub", reg, "fp", resultReg);
		} else {
			// length is stored in gloabl variable
			tempReg = func->getFreeRegs().front();
			addCommand(func, "ldr", tempReg, resultReg);
			addCommand(func, "sub", reg, "fp", tempReg);
		}
	} else {
		int len = ExprGen::evaluateExpression(length->getRoot(), st);

		if (type->getTypeName() == "Number") {
			len *= 4;
		}
		func->increaseStackPointer(len);

		// make it point len down the stack
		addCommand(func, "sub", reg, "fp", "#" + boost::lexical_cast<string>(len));
	}

	if (onStack) {
		func->increaseStackPointer(4);
		string sp = boost::lexical_cast<string>(func->getStackPointer());

		addCommand(func, "str", reg, "[fp, #-" + sp + "]");
		addCommand(func, "pop", "{r0}");

		arr->setAssLoc("[fp, #-" + sp + "]");
	}
}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
								 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st) {
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
