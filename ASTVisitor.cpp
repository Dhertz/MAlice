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
	initFreeRegs();

	vector<string> alignArg;
	alignArg.push_back("2");
	_endDefs.push_back(AssemCom(".align", 1, alignArg));						// .align 2
}

void ASTVisitor::initFreeRegs() {
	string regs[] = {"r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};

	_freeRegs = vector<string>(regs, regs + sizeof(regs) / sizeof(string));
}

void ASTVisitor::visitProg(vector <boost::shared_ptr<ASTNode> > children) {
	// I imagine some bollocks will go here...
	
	vector<boost::shared_ptr<ASTNode> >::iterator i;
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this());
	}

	// And here...
}

void ASTVisitor::visitProcDec(string name, 
								boost::shared_ptr<HeaderParamsAST> params,
								vector <boost::shared_ptr<ASTNode> > children, 
								boost::shared_ptr<SymbolTable> st) {

	vector<int> callableKids;

	if (name == "hatta") {
		boost::shared_ptr<AssemFunc> func(new AssemFunc());
		_functions.push_back(func);

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

	vector<int> callableKids;

	boost::shared_ptr<AssemFunc> func(new AssemFunc());
	_functions.push_back(func);

	int i = 0;
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
				func->addBack(".comm", comm);
				Label l;
				func->addBack(l.getLabel() + ":", std::vector<string>());
				std::vector<string> word;
				word.push_back(varName);
				func->addBack(".word", word);
				var->setAssLoc(l.getLabel());
			} else if (varType->getTypeName() == "Letter") {
				std::vector<string> comm;
				comm.push_back(varName);
				comm.push_back("1");
				func->addBack(".comm", comm);
				Label l;
				func->addBack(l.getLabel() + ":", std::vector<string>());
				std::vector<string> word;
				word.push_back(varName);
				func->addBack(".word", word);
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
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	func->addListBack(res.get<1>());
	_freeRegs = res.get<2>();

	vector<string> incArgs(2, resultReg);
	incArgs.push_back("#1");

	func->addBack("add", incArgs);												// add resultReg resultReg #1
}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr,
							boost::shared_ptr<SymbolTable> st,
							boost::shared_ptr<AssemFunc> func) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	func->addListBack(res.get<1>());
	_freeRegs = res.get<2>();

	vector<string> incArgs(2, resultReg);
	incArgs.push_back("#1");

	func->addBack("dec", incArgs);												// add resultReg resultReg #1

}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);

	_freeRegs = res.get<2>();
	string resultReg = res.get<0>();

	Label strLbl;
	if (resultReg[0] != '.') {
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":", 0 , std::vector<string>()));		// strLbl:
	}

	if (expr->getType()->getTypeName() == "Sentence") {
		if (resultReg[0] != '.') {
			vector<string> asciiArg;
			asciiArg.push_back(resultReg);
			_endDefs.push_back(AssemCom(".asciz", 1, asciiArg));
		}
	} else if (resultReg != "r1") {
		vector<string> asciiArg;
		asciiArg.push_back("\"%i\"");
		_endDefs.push_back(AssemCom(".asciz", 1, asciiArg));

		func->addListBack(res.get<1>());											// expr instrs

		vector<string> movArgs;
		movArgs.push_back("r1");
		movArgs.push_back(resultReg);
		func->addBack("mov", movArgs);											// mov r1 resultReg
	} else {
		func->addListBack(res.get<1>());											// expr instrs
	}

	vector<string> ldrArgs;
	ldrArgs.push_back("r0");
	if (resultReg[0] != '.') {
		ldrArgs.push_back("=" + strLbl.getLabel());								
	} else {
		ldrArgs.push_back("=" + resultReg);	
	}
	func->addBack("ldr", ldrArgs);												// ldr r0 =strLbl

	vector<string> printArg;
	printArg.push_back("printf");
	func->addBack("bl", printArg);												// bl printf

}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	// I don't think I care about the expression, that can be handled in the 
	// function call

	vector<string> retArg;
	retArg.push_back("lr");

	func->addBack("bx", retArg);													// bx lr
}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st,
							  boost::shared_ptr<AssemFunc> func) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);

	_freeRegs = res.get<2>();
	string resultReg = res.get<0>();

	// non-sentence case atm

	if (expr->getType()->getTypeName() == "String"){
		
		vector<string> subr0Args;
		subr0Args.push_back("r0");
		subr0Args.push_back("fp");
		subr0Args.push_back("#260");
		func->addBack("sub", subr0Args);											// sub r0, fp, #260
		func->increaseStackPointer(260);

		vector<string> printArg;
		printArg.push_back("__isoc99_scanf");
		func->addBack("bl", printArg);											// bl __isoc99_scanf

		vector<string> ldrArgs;
		ldrArgs.push_back(resultReg);
		ldrArgs.push_back("[fp, #-260]");
		func->addBack("ldr", ldrArgs);											//ldr resultReg, [fp, #-260]

	} else {
		
		vector<string> strArgs;
		strArgs.push_back(resultReg);
		strArgs.push_back("[fp, #-8]");
		func->addBack("str", strArgs);											// str resultReg [fp, #-8]
		Label strLbl;
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":", 0 , std::vector<string>()));		// strLbl:
		vector<string> asciiArg;

		if (expr->getType()->getTypeName() == "Number") {	
			asciiArg.push_back("\"%i\"");										// .asciz "%i"
		} else if (expr->getType()->getTypeName() == "Letter") {
			asciiArg.push_back("\"%c\"");										// .asciz "%c"
		}
		
		_endDefs.push_back(AssemCom(".asciz", 1, asciiArg));

		vector<string> ldr0Args;
		ldr0Args.push_back("r0");
		ldr0Args.push_back("=" + strLbl.getLabel());
		func->addBack("ldr", ldr0Args);											// ldr r0 =strLbl

		vector<string> ldr1Args;
		ldr1Args.push_back("r1");
		ldr1Args.push_back("[fp, #-8]");
		func->addBack("ldr", ldr1Args);											// ldr r1 [fp, #-8]

		vector<string> printArg;
		printArg.push_back("__isoc99_scanf");
		func->addBack("bl", printArg);											// bl __isoc99_scanf

		vector<string> ldrArgs;
		ldrArgs.push_back(resultReg);
		ldrArgs.push_back("[fp, #-8]");
		func->addBack("ldr", ldrArgs);											// ldr resultReg [fp, #-8]
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
		(*i)->accept(shared_from_this());
	}

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	_freeRegs = res.get<2>();

	func->addListBack(res.get<1>());												// expr instrs

	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	func->addBack("cmp", cmpArgs);												// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(loopLabel.getLabel());
	func->addBack("bne", bneArgs);												// bne loop
}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody, 
							   boost::shared_ptr<SymbolTable> st,
							   boost::shared_ptr<AssemFunc> func) {
	Label elseLabel;

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	_freeRegs = res.get<2>();

	std::vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	func->addBack("cmp", cmpArgs);												// cmp resultReg #0

	std::vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());
	func->addBack("bne", bneArgs);												// bne else

	trueBody->accept(shared_from_this(), func);										// if body

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

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	func->addListBack(res.get<1>());
	_freeRegs = res.get<2>();

	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	func->addBack("cmp", cmpArgs);												// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());
	func->addBack("bne", bneArgs);												// bne else

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

	string loc = var->getAssLoc();

	boost::tuple< string, list<AssemCom>, vector<string> > res 
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	if (var->getTypeName()->getTypeName() == "Sentence") {
		vector<string> asciiArg;
		asciiArg.push_back(res.get<0>());
		Label strLbl;
		_endDefs.push_back(
			AssemCom(strLbl.getLabel() + ":", 0 , std::vector<string>()));
		var->setAssLoc(strLbl.getLabel());
		_endDefs.push_back(AssemCom(".asciz", 1, asciiArg));
	} else {
		string rhs = res.get<0>();
		_freeRegs = res.get<2>();
		func->addListBack(res.get<1>());											// expr instrs

		if (loc == "") {
			if (_freeRegs.empty()) {
				// TODO: memory allocation
				loc = "TODO";
			} else {
				var->setAssLoc(_freeRegs.front());
				_freeRegs.erase(_freeRegs.begin());
			}
		}

		// mov loc, rhs
		vector<string> args;
		args.push_back(var->getAssLoc());
		args.push_back(rhs);
		func->addBack("mov", args);												// mov var rhs
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

		boost::tuple< string, list<AssemCom>, vector<string> > genParam
	  	  = ExprGen::generateExpression(cp, st, _freeRegs);

	  	string paramLoc = genParam.get<0>();
		func->addListBack(genParam.get<1>());
		_freeRegs = genParam.get<2>();

	  	if (i < 4) {
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
				func->addBack("push", args);										// push {ri}
			} else if (_freeRegs.empty()) {
				// Need to temporarily borrow a register
				vector<string> args;
				args.push_back("{r0}");
				func->addBack("push", args);										// push {r0}

				args.clear();
				args.push_back("r0");
				args.push_back(paramLoc);
				func->addBack("mov", args);										// mov r0 ri

				// WHy is this here? - Owen
				//func->addBack("push");											// push {r0}

				args.clear();
				args.push_back("{r0}");
				func->addBack("pop", args);										// pop {r0}
			} else {
				string reg = _freeRegs.front();

				vector<string> args;
				args.push_back(reg);
				args.push_back(paramLoc);
				func->addBack("mov", args);										// mov reg paramLoc

				args.clear();
				args.push_back("{" + reg + "}");
				func->addBack("push", args);										// push reg
			}
	  	}
	  	i++;
	}

	vector<string> blArgs;
	blArgs.push_back(name);
	func->addBack("bl", blArgs);													// bl name
}

void ASTVisitor::visitArrayAssign(string name,
                  					boost::shared_ptr<ExprAST> index,
                  					boost::shared_ptr<ExprAST> value, 
							        boost::shared_ptr<SymbolTable> st,
							        boost::shared_ptr<AssemFunc> func) {}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
								 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st,
							     boost::shared_ptr<AssemFunc> func) {
	boost::shared_ptr<Identifier> arrIdent = 
										_globalSt->lookupCurrLevelOnly(name);
	if (arrIdent) {
		boost::shared_ptr<Array> arr =  
			boost::shared_polymorphic_downcast<Array>(arrIdent);
		boost::shared_ptr<Type> arrType = arr->getElemType();
		if (arrType->getTypeName() == "Number") {
			std::vector<string> comm;
			comm.push_back(name);
			comm.push_back("4*something"); 										//need to access array length here.
			func->addBack(".comm", comm);
			Label l;
			func->addBack(l.getLabel() + ":", std::vector<string>());
			std::vector<string> word;
			word.push_back(name);
			func->addBack(".word", word);
			arr->setAssLoc(l.getLabel());																	
		} else if (arrType->getTypeName() == "Letter") {
			std::vector<string> comm;
			comm.push_back(name);
			comm.push_back("1*something"); 										//need to access array length here.
			func->addBack(".comm", comm);
			Label l;
			func->addBack(l.getLabel() + ":", std::vector<string>());
			std::vector<string> word;
			word.push_back(name);
			func->addBack(".word", word);	
			arr->setAssLoc(l.getLabel());
		} else {
			// TODO: string case
		}
	}
}

vector<boost::shared_ptr<AssemFunc> > ASTVisitor::getFunctions() {
	return _functions;
}

list<AssemCom> ASTVisitor::getEndDefs() {
	return _endDefs;
}

void ASTVisitor::printFreeRegs() {
	std::vector<string>::iterator it;
	for (it = _freeRegs.begin(); it != _freeRegs.end(); ++it) {
		cout << *it << endl;
	}
}
