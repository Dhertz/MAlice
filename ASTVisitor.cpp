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
	vector<boost::shared_ptr<ASTNode> >::iterator it;							// function body
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
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs());
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	vector<string> incArgs(2, resultReg);
	incArgs.push_back("#1");

	func->addBack("add", incArgs);												// add resultReg resultReg #1
}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr,
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs());
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	vector<string> incArgs(2, resultReg);
	incArgs.push_back("#1");

	func->addBack("dec", incArgs);												// add resultReg resultReg #1

}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs());

	func->setFreeRegs(res->get<2>());
	string resultReg = res->get<0>();

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

		if (resultReg[0] != '.') {
			// local variable or expression

			func->addListBack(res->get<1>());									// expr instrs

			if (resultReg != "r1") {
				if (!func->regIsFree("r1")) {
					vector<string> pushArg;
					pushArg.push_back("{r1}");
					func->addBack("push", pushArg);	
				}																// push {r1}

				vector<string> movArgs;
				movArgs.push_back("r1");
				movArgs.push_back(resultReg);
				func->addBack("mov", movArgs);									// mov r1 resultReg
			}
		} else {
			//global varaible
			if (!func->regIsFree("r1")) {
				vector<string> pushArg;
				pushArg.push_back("{r1}");
				func->addBack("push", pushArg);	
			}

			vector<string> ldrArgs;
			ldrArgs.push_back("r1");
			ldrArgs.push_back(resultReg);
			func->addBack("ldr", ldrArgs);

			vector<string> ldr1Args;
			ldr1Args.push_back("r1");
			ldr1Args.push_back("[r1]");
			func->addBack("ldr", ldr1Args);										// mov ldr resultReg
		}

	}

	if (!func->regIsFree("r0") && resultReg[0] == '\"') {
		vector<string> push0Arg;
		push0Arg.push_back("{r0}");
		func->addBack("push", push0Arg);										// push {r0}
	}

	if ((expr->getType()->getTypeName() == "Sentence" && resultReg[0] == '\"') 
		|| !(expr->getType()->getTypeName() == "Sentence")) {
		vector<string> ldrArgs;
		ldrArgs.push_back("r0");
		ldrArgs.push_back("=" + strLbl.getLabel());
		func->addBack("ldr", ldrArgs);											// ldr r0 =strLbl
	}

	vector<string> printArg;
	printArg.push_back("printf");
	func->addBack("bl", printArg);												// bl printf

	if (!func->regIsFree("r0") && resultReg[0] == '\"') {
		vector<string> pop0Arg;
		pop0Arg.push_back("{r0}");
		func->addBack("pop", pop0Arg);											// pop {r0}
	}
	
	if (!func->regIsFree("r1")) {
		if (resultReg != "r1" && expr->getType()->getTypeName() != "Sentence") {
			vector<string> pop1Arg;
			pop1Arg.push_back("{r1}");
			func->addBack("pop", pop1Arg);										// pop {r1}
		}
	}
}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs());

	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	if (resultReg != "r0") {
		vector<string> movArgs;
		movArgs.push_back("r0");
		movArgs.push_back(resultReg);
		func->addBack("mov", movArgs);											// mov r0 resultReg
	}

	vector<string> retArg;
	retArg.push_back("lr");
	func->addBack("bx", retArg);												// bx lr
}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs());

	func->setFreeRegs(res->get<2>());
	string resultReg = res->get<0>();

	if (expr->getType()->getTypeName() == "Sentence"){
		
		func->increaseStackPointer(260);
		string sp = boost::lexical_cast<string>(func->getStackPointer());

		vector<string> subr0Args;
		subr0Args.push_back("r0");
		subr0Args.push_back("fp");
		subr0Args.push_back("#" + sp);
		func->addBack("sub", subr0Args);										// sub r0, fp, #sp

		vector<string> printArg;
		printArg.push_back("__isoc99_scanf");
		func->addBack("bl", printArg);											// bl __isoc99_scanf

		vector<string> ldrArgs;
		ldrArgs.push_back(resultReg);
		ldrArgs.push_back("[fp, #-" + sp + "]");
		func->addBack("ldr", ldrArgs);											//ldr resultReg, [fp, #-sp]

	} else {
		func->increaseStackPointer(8);
		string sp = boost::lexical_cast<string>(func->getStackPointer());

		vector<string> strArgs;
		strArgs.push_back(resultReg);
		strArgs.push_back("[fp, #-" + sp + "]");
		func->addBack("str", strArgs);											// str resultReg [fp, #-sp]
		Label strLbl;
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":" , std::vector<string>()));			// strLbl:
		vector<string> asciiArg;

		if (expr->getType()->getTypeName() == "Number") {	
			asciiArg.push_back("\"%i\"");										// .asciz "%i"
		} else if (expr->getType()->getTypeName() == "Letter") {
			asciiArg.push_back("\"%c\"");										// .asciz "%c"
		}
		
		_endDefs.push_back(AssemCom(".asciz", asciiArg));

		vector<string> ldr0Args;
		ldr0Args.push_back("r0");
		ldr0Args.push_back(strLbl.getLabel());
		func->addBack("ldr", ldr0Args);											// ldr r0 strLbl

		vector<string> sub1Args;
		sub1Args.push_back("r0");
		sub1Args.push_back("fp");
		sub1Args.push_back("#" + sp);
		func->addBack("sub", sub1Args);											// sub r1, fp, sp

		vector<string> printArg;
		printArg.push_back("__isoc99_scanf");
		func->addBack("bl", printArg);											// bl __isoc99_scanf

		vector<string> ldrArgs;
		ldrArgs.push_back(resultReg);
		ldrArgs.push_back("[fp, #-" + sp + "]");
		func->addBack("ldr", ldrArgs);											// ldr resultReg [fp, #-sp]
	}
}

void ASTVisitor::visitWhile(boost::shared_ptr<ExprAST> cond, 
			   			   	  vector <boost::shared_ptr<ASTNode> > children, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {
	Label loopLabel;

	func->addBack(loopLabel.getLabel() + ":", std::vector<string>());			// loop:

	vector<boost::shared_ptr<ASTNode> >::iterator i;							// loop body
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this(), func);
	}

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(cond->getRoot(), st, func->getFreeRegs());
	string resultReg = res->get<0>();
	func->setFreeRegs(res->get<2>());
	func->addListBack(res->get<1>());											// expr instrs

	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	func->addBack("cmp", cmpArgs);												// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(loopLabel.getLabel());
	func->addBack("beq", bneArgs);												// bne loop
}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	Label elseLabel;

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(cond->getRoot(), st, func->getFreeRegs());
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	std::vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	func->addBack("cmp", cmpArgs);												// cmp resultReg #0

	std::vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());
	func->addBack("beq", bneArgs);												// bne else

	trueBody->accept(shared_from_this(), func);									// if body

	Label endLabel;
	vector<string> bArgs;
	bArgs.push_back(endLabel.getLabel());
	func->addBack("b", bArgs);													// b end

	func->addBack(elseLabel.getLabel() + ":", std::vector<string>());			// else:

	falseBody->accept(shared_from_this(), func);								// else body

	func->addBack(elseLabel.getLabel() + ":", std::vector<string>());			// end:
}

void ASTVisitor::visitIf(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children, 
						   boost::shared_ptr<SymbolTable> st,
						   boost::shared_ptr<AssemFunc> func) {

	Label elseLabel;

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res
	  = ExprGen::generateExpression(cond->getRoot(), st, func->getFreeRegs());

	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	func->setFreeRegs(res->get<2>());

	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	func->addBack("cmp", cmpArgs);												// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());
	func->addBack("beq", bneArgs);												// bne else

	trueBody->accept(shared_from_this(), func);										// if body

	if (children.size() == 1) {
		func->addBack(elseLabel.getLabel() + ":", std::vector<string>());		// end:

	} else {
		Label endLabel;

		vector<string> bArgs;
		bArgs.push_back(endLabel.getLabel());
		func->addBack("b", bArgs);												// b end

		func->addBack(elseLabel.getLabel() + ":", std::vector<string>());		// else:

		(*(children.begin() + 1))->accept(shared_from_this(), func);	

		func->addBack(endLabel.getLabel() + ":", std::vector<string>());		// end:
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
	  = ExprGen::generateExpression(expr->getRoot(), st, func->getFreeRegs());

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

		if (loc == "") {
			if (func->getFreeRegs().empty()) {
				// TODO: memory allocation
				loc = "TODO";
			} else {
				var->setAssLoc(rhs);
				func->removeReg(var->getAssLoc());
			}
		} else if (loc[0] == '.') {
			// global variable assignment
			string wordLoc;
			vector<string> ldArgs;

			if (func->getFreeRegs().empty()) {
				// out of registers
				wordLoc = "TODO";
			} else {
				wordLoc = func->getFreeRegs().front();
			}

			ldArgs.push_back(wordLoc);
			ldArgs.push_back(loc);
			func->addBack("ldr", ldArgs);										// ldr wordLoc loc

			if (var->getTypeName()->getTypeName() == "Letter") {
				vector<string> strbArgs;
				strbArgs.push_back(rhs);
				strbArgs.push_back("[" + wordLoc + ", #0]");
				func->addBack("strb", strbArgs);								// strb rhs [wordloc]
			} else {
				// must be a global number
				vector<string> strbArgs;
				strbArgs.push_back(rhs);
				strbArgs.push_back("[" + wordLoc + ", #0]");
				func->addBack("str", strbArgs);									// str rhs [wordloc]
			}
		} else {
			vector<string> args;
			args.push_back(var->getAssLoc());
			args.push_back(rhs);
			func->addBack("mov", args);											// mov var rhs
		}
	}
	
}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st) {
	cout << "hello" << endl;
	// Global inline assignments

	string regs[] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};

	vector<string> freeRegs =
		vector<string>(regs, regs + sizeof(regs) / sizeof(string));

	boost::shared_ptr<Identifier> varIdent 
	  = st->lookupCurrLevelAndEnclosingLevels(varName);
	boost::shared_ptr<Variable> var 
	  = boost::shared_polymorphic_downcast<Variable>(varIdent);

	string loc = var->getAssLoc();

	vector<string> ldArgs;

	ldArgs.push_back("r0");
	ldArgs.push_back(loc);
	_globalInlines.push_back(AssemCom("ldr", ldArgs));							// ldr wordLoc loc

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > res 
	  = ExprGen::generateExpression(expr->getRoot(), st, freeRegs);
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
		string wordLoc = "r0"; // fin since it's always going to be the first thing
		

		if (var->getTypeName()->getTypeName() == "Letter") {
			vector<string> strbArgs;
			strbArgs.push_back(rhs);
			strbArgs.push_back("[" + wordLoc + ", #0]");
			_globalInlines.push_back(AssemCom("strb", strbArgs));				// strb rhs [wordloc]
		} else {
			// must be a global number
			var->setVal(ExprGen::evaluateExpression(expr->getRoot(), st));
			vector<string> strbArgs;
			strbArgs.push_back(rhs);
			strbArgs.push_back("[" + wordLoc + ", #0]");
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
	  	  = ExprGen::generateExpression(cp, st, func->getFreeRegs());

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
  			vector<string> args;
			args.push_back("r" + boost::lexical_cast<string>(i));
			args.push_back("=" + strLbl.getLabel());
			func->addBack("ldr", args);

		} else if (i < 4) {
	  		if (paramLoc != "r" + boost::lexical_cast<string>(i)) {
	  			//Parameter needs to be moved into correct register
	  			vector<string> args;
				args.push_back("r" + boost::lexical_cast<string>(i));
				args.push_back(paramLoc);
				func->addBack("mov", args);
	  		}
	  	} else {
	  		// Push any other params
			if (paramLoc[0] == 'r') {
				vector<string> args;
				args.push_back("{" + paramLoc + "}");
				func->addBack("push", args);									// push {ri}
			} else if (func->getFreeRegs().empty()) {
				// Need to temporarily borrow a register
				vector<string> args;
				args.push_back("{r0}");
				func->addBack("push", args);									// push {r0}

				args.clear();
				args.push_back("r0");
				args.push_back(paramLoc);
				func->addBack("mov", args);										// mov r0 ri

				// WHy is this here? - Owen
				//func->addBack("push");										// push {r0}

				args.clear();
				args.push_back("{r0}");
				func->addBack("pop", args);										// pop {r0}
			} else {
				string reg = func->getFreeRegs().front();

				vector<string> args;
				args.push_back(reg);
				args.push_back(paramLoc);
				func->addBack("mov", args);										// mov reg paramLoc

				args.clear();
				args.push_back("{" + reg + "}");
				func->addBack("push", args);									// push reg
			}
	  	}
	  	i++;
	}

	vector<string> blArgs;
	blArgs.push_back(name);
	func->addBack("bl", blArgs);												// bl name
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
	  			= ExprGen::generateExpression(index->getRoot(), st, func->getFreeRegs());
	
	string resultReg = ind->get<0>();

	//make size bigger for integers
	if (arr->getTypeName() == "Number") {	
		std::vector<string> mul;
		mul.push_back(resultReg);												
		mul.push_back("4");														//mul resReg, 4, resReg
		mul.push_back(resultReg);
		func->addBack("mul", mul);		
	}
	
	string reg = arr->getAssLoc();

	boost::shared_ptr< boost::tuple< string, list<AssemCom>, vector<string> > > val
	  		= ExprGen::generateExpression(value->getRoot(), st, func->getFreeRegs());
	string valReg = val->get<0>();
	func->addListBack(val->get<1>());
		
	std::vector<string> str;
	str.push_back(valReg);
	str.push_back("[" + resultReg + ", " + reg + "]");
	func->addBack("str", str);													// str valReg [resultReg, reg]
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
  			= ExprGen::generateExpression(length->getRoot(), st, func->getFreeRegs());
	string resultReg = res->get<0>();
	func->addListBack(res->get<1>());
	
	//make it bigger for integers
	if (type->getTypeName() == "Number") {
		std::vector<string> mul;
		mul.push_back(resultReg);
		mul.push_back("4");
		mul.push_back(resultReg);
		func->addBack("mul", mul);											//mul resReg, 4, resReg
	}

	string reg = func->getFreeRegs().front();
	if (func->getFreeRegs().empty()) {
		// TODO: memory allocation
		reg = "TODO";
	} else {
		reg = func->getFreeRegs().front();
		func->removeReg(reg);
	}
	arr->setAssLoc(reg);

	int len = ExprGen::evaluateExpression(length->getRoot(), st);

	if (type->getTypeName() == "Number") {
		len *= 4;
	}

	func->increaseStackPointer(len);

	//make it point somewhere down the stack
	vector<string> sub;
	sub.push_back(reg);
	sub.push_back("fp");
	sub.push_back(resultReg);
	func->addBack("sub", sub);

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
	//assert(len == 0);
	
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
