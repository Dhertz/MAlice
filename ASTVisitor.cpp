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
#include <sstream>

ASTVisitor::ASTVisitor(boost::shared_ptr<SymbolTable> st) {
	_globalSt = st;
}

void ASTVisitor::initFreeRegs() {
	string regs[] = {"r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};

	_freeRegs = vector<string>(regs, regs + sizeof(regs) / sizeof(string));
}

void ASTVisitor::visitProg(vector <boost::shared_ptr<ASTNode> > children, 
							 boost::shared_ptr<SymbolTable> st) {
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

	if (name == "hatta") {
		vector<string> alignArg;
		alignArg.push_back("2");
		_instrs.push_back(AssemCom(".align", 1, alignArg));						// .align 2

		vector<string> globalArg;
		globalArg.push_back("main");
		_instrs.push_back(AssemCom(".global", 1, globalArg));					// .global main

		_instrs.push_back(AssemCom("main:", 0, std::vector<string>()));			// main:

		vector<string> pushArg;
		pushArg.push_back("{lr}");
		_instrs.push_back(AssemCom("push", 1, pushArg));						// push {lr}

		vector<int> callableKids;

		int i = 0;
		vector<boost::shared_ptr<ASTNode> >::iterator it;						// function body
		for (it = children.begin(); it != children.end(); ++it) {
			if ((*it)->getNodeName() == "FuncDec" 
					|| (*it)->getNodeName() == "ProcDec") {
				callableChildren.push_back(i);
			} else {
				(*it)->accept(shared_from_this());
			}
			i++;
		}

		vector<string> returnArg;
		returnArg.push_back("r0");
		returnArg.push_back("#0");
		_instrs.push_back(AssemCom("mov", 2, returnArg));						// mov r0, #0

		vector<string> popArg;
		popArg.push_back("{pc}");
		_instrs.push_back(AssemCom("pop", 1, popArg));							// pop {pc}

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
	vector<string> alignArg;
	alignArg.push_back("2");
	_instrs.push_back(AssemCom(".align", 1, alignArg));							// .align 2

	_instrs.push_back(AssemCom(name + ":", 0, std::vector<string>()));			// name:

	vector<string> stmfdArgs;
	stmfdArgs.push_back("sp!");
	stmfdArgs.push_back("{r4-r10, fp, lr}");
	_instrs.push_back(AssemCom("stmfd", 2, stmfdArgs));							// stmfd sp!, {r4-r10, fp, lr}

	vector<string> fpArgs;
	fpArgs.push_back("fp");
	fpArgs.push_back("sp");
	fpArgs.push_back("#256");
	_instrs.push_back(AssemCom("add", 3, fpArgs));								// add fp, sp, #256

	vector<string> subArgs;
	subArgs.push_back("sp");
	subArgs.push_back("sp");
	subArgs.push_back("#260");
	_instrs.push_back(AssemCom("add", 3, subArgs));								// sub sp, sp, #260

	vector<int> callableChildren;

	int i = 0;
	vector<boost::shared_ptr<ASTNode> >::iterator it;							// function body
	for (it = children.begin(); it != children.end(); ++it) {
		if ((*it)->getNodeName() == "FuncDec" 
				|| (*it)->getNodeName() == "ProcDec") {
			callableChildren.push_back(i);
		} else {
			(*it)->accept(shared_from_this());
		}
		i++;
	}

	vector<string> spArgs;
	spArgs.push_back("sp");
	spArgs.push_back("fp");
	spArgs.push_back("#256");
	_instrs.push_back(AssemCom("sub", 3, spArgs));								// sub sp, fp, #256

	vector<string> ldmfdArgs;
	ldmfdArgs.push_back("sp!");
	ldmfdArgs.push_back("{r4-r10, fp, pc}");
	_instrs.push_back(AssemCom("ldmfd", 2, ldmfdArgs));							// ldmfd sp!, {r4-r10, fp, pc}

	// moved nested functions outside of parent function
	vector<int>::iterator cIt;
	for (cIt = callableChildren.begin(); cIt != callableChildren.end(); ++cIt) {
		(*(children.begin() + (*cIt)))->accept(shared_from_this());				
	}
}

void ASTVisitor::visitVarDec(string typeName, string varName, 
							   boost::shared_ptr<SymbolTable> st) {
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
				_instrs.push_back(AssemCom(".comm", 2, comm));
				Label l;
				_instrs.push_back(AssemCom(l.getLabel() + ":", 0, 
												std::vector<string>()));
				std::vector<string> word;
				word.push_back(varName);
				_instrs.push_back(AssemCom(".word", 1, word));
				var->setAssLoc(l.getLabel());
			} else if (varType->getTypeName() == "Letter") {
				std::vector<string> comm;
				comm.push_back(varName);
				comm.push_back("1");
				_instrs.push_back(AssemCom(".comm", 2, comm));
				Label l;
				_instrs.push_back(AssemCom(l.getLabel() + ":", 0,
												std::vector<string>()));
				std::vector<string> word;
				word.push_back(varName);
				_instrs.push_back(AssemCom(".word", 1, word));
				var->setAssLoc(l.getLabel());
			} else {
				// TODO: string case
			}
		}
	}
}

void ASTVisitor::visitInc(boost::shared_ptr<ExprAST> expr, 
							boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();

	list<AssemCom> exprInstrs = res.get<1>();
	_instrs.splice(_instrs.end(), exprInstrs);

	_freeRegs = res.get<2>();

	vector<string> incArgs;
	incArgs.push_back(resultReg);
	incArgs.push_back(resultReg);
	incArgs.push_back("#1");

	_instrs.push_back(AssemCom("add", 3, incArgs));								// add resultReg resultReg #1
}

void ASTVisitor::visitDec(boost::shared_ptr<ExprAST> expr,
							boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();

	list<AssemCom> exprInstrs = res.get<1>();
	_instrs.splice(_instrs.end(), exprInstrs);

	_freeRegs = res.get<2>();

	vector<string> decArgs;
	decArgs.push_back(resultReg);
	decArgs.push_back(resultReg);
	decArgs.push_back("#1");

	// May need to be "subs"
	_instrs.push_back(AssemCom("sub", 3, decArgs));								// sub resultReg resultReg #1
}

void ASTVisitor::visitPrint(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);

	_freeRegs = res.get<2>();
	string resultReg = res.get<0>();
	if (resultReg != "r0") {
		vector<string> push;
		push.push_back("{r0}");
		_instrs.push_back(AssemCom("push", 1, push));							// push {r0}

		list<AssemCom> exprInstrs = res.get<1>();
		_instrs.splice(_instrs.end(), exprInstrs);								// expr isntrs

		vector<string> movArgs;
		movArgs.push_back("r0");
		movArgs.push_back(resultReg);
		_instrs.push_back(AssemCom("mov", 2, movArgs));							// mov r0 resultReg

		vector<string> printArg;
		printArg.push_back("printf");

		_instrs.push_back(AssemCom("bl", 1, printArg));							// bl printf
		_instrs.push_back(AssemCom("pop", 1, push));							// pop {r0}
	} else {
		list<AssemCom> exprInstrs = res.get<1>();
		_instrs.splice(_instrs.end(), exprInstrs);								// expr instrs

		vector<string> printArg;
		printArg.push_back("printf");

		_instrs.push_back(AssemCom("bl", 1, printArg));							// bl printf
	}
}

void ASTVisitor::visitReturn(boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st) {
	// I don't think I care about the expression, that can be handled in the 
	// function call

	vector<string> retArg;
	retArg.push_back("lr");

	_instrs.push_back(AssemCom("bx", 1, retArg));								// bx lr
}

void ASTVisitor::visitStdin(boost::shared_ptr<ExprAST> expr, 
							  boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);

	_freeRegs = res.get<2>();
	string resultReg = res.get<0>();
	if (resultReg != "r0") {
		
		vector<string> push;
		push.push_back("{r0}");
		_instrs.push_back(AssemCom("push", 1, push));

		list<AssemCom> exprInstrs = res.get<1>();

		_instrs.splice(_instrs.end(), exprInstrs);

		vector<string> movArg;
		movArg.push_back("r0");
		movArg.push_back("#1024");
		_instrs.push_back(AssemCom("mov", 2, movArg));
		
		vector<string> mallocArg;
		mallocArg.push_back("malloc");

		_instrs.push_back(AssemCom("bl", 1, mallocArg));

		vector<string> stdinArg;
		stdinArg.push_back("gets");

		vector<string> mov1Arg;
		mov1Arg.push_back(resultReg);
		mov1Arg.push_back("r0");
		_instrs.push_back(AssemCom("mov", 2, mov1Arg));
		mov1Arg.push_back("malloc");

		_instrs.push_back(AssemCom("bl", 1, stdinArg));
		
		_instrs.push_back(AssemCom("pop", 1, push));
	
	} else {
		
		list<AssemCom> exprInstrs = res.get<1>();

		_instrs.splice(_instrs.end(), exprInstrs);

		vector<string> mallocArg;
		mallocArg.push_back("malloc");

		_instrs.push_back(AssemCom("bl", 1, mallocArg));

		vector<string> stdinArg;
		stdinArg.push_back("gets");

		_instrs.push_back(AssemCom("bl", 1, stdinArg));
	}
}

void ASTVisitor::visitWhile(boost::shared_ptr<ExprAST> cond, 
			   			   	  vector <boost::shared_ptr<ASTNode> > children, 
							  boost::shared_ptr<SymbolTable> st) {
	Label loopLabel;

	_instrs.push_back(
	   AssemCom(loopLabel.getLabel() + ":", 0, std::vector<string>()));			// loop:

	vector<boost::shared_ptr<ASTNode> >::iterator i;							// loop body
	for (i = children.begin(); i != children.end(); ++i) {
		(*i)->accept(shared_from_this());
	}

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	_freeRegs = res.get<2>();

	list<AssemCom> condInstrs = res.get<1>();
	_instrs.splice(_instrs.end(), condInstrs);

	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");

	_instrs.push_back(AssemCom("cmp", 2, cmpArgs));								// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(loopLabel.getLabel());

	_instrs.push_back(AssemCom("bne", 1, bneArgs));								// bne loop
}

void ASTVisitor::visitChoice(boost::shared_ptr<ExprAST> cond, 
				   			   boost::shared_ptr<IfBodyAST> trueBody, 
				   			   boost::shared_ptr<IfBodyAST> falseBody, 
							   boost::shared_ptr<SymbolTable> st) {
	Label elseLabel;

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	_freeRegs = res.get<2>();

	list<AssemCom> condInstrs = res.get<1>();
	_instrs.splice(_instrs.end(), condInstrs);

	std::vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");

	_instrs.push_back(AssemCom("cmp", 2, cmpArgs));								// cmp resultReg #0

	std::vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());

	_instrs.push_back(AssemCom("bne", 1, bneArgs));								// bne else

	trueBody->accept(shared_from_this());										// if body

	Label endLabel;
	vector<string> bArgs;
	bArgs.push_back(endLabel.getLabel());
	_instrs.push_back(AssemCom("b", 1, bArgs));									// b end

	_instrs.push_back(
	   AssemCom(elseLabel.getLabel() + ":", 0, std::vector<string>()));			// else:

	falseBody->accept(shared_from_this());										// else body

	_instrs.push_back(
	   AssemCom(endLabel.getLabel() + ":", 0, std::vector<string>()));			// end:
}

void ASTVisitor::visitIf(boost::shared_ptr<ExprAST> cond,
			   			   boost::shared_ptr<IfBodyAST> trueBody, 
			   			   vector <boost::shared_ptr<ASTNode> > children, 
						   boost::shared_ptr<SymbolTable> st) {

	Label elseLabel;

	boost::tuple< string, list<AssemCom>, vector<string> > res
	  = ExprGen::generateExpression(cond->getRoot(), st, _freeRegs);
	string resultReg = res.get<0>();
	_freeRegs = res.get<2>();

	list<AssemCom> condInstrs = res.get<1>();
	_instrs.splice(_instrs.end(), condInstrs);

	vector<string> cmpArgs;
	cmpArgs.push_back(resultReg);
	cmpArgs.push_back("#0");
	_instrs.push_back(AssemCom("cmp", 2, cmpArgs));								// cmp resultReg #0

	vector<string> bneArgs;
	bneArgs.push_back(elseLabel.getLabel());
	_instrs.push_back(AssemCom("bne", 1, bneArgs));								// bne else

	(*children.begin())->accept(shared_from_this());							// if body

	if (children.size() == 1) {
		_instrs.push_back(
	   		AssemCom(elseLabel.getLabel() + ":", 0, std::vector<string>()));	// end:

	} else {
		Label endLabel;

		vector<string> bArgs;
		bArgs.push_back(endLabel.getLabel());
		_instrs.push_back(AssemCom("b", 1, bArgs));								// b end

		_instrs.push_back(
	   		AssemCom(elseLabel.getLabel() + ":", 0, std::vector<string>()));	// else:

		(*(children.begin() + 1))->accept(shared_from_this());	

		_instrs.push_back(
	   		AssemCom(endLabel.getLabel() + ":", 0, std::vector<string>()));		// end:
	}
}

void ASTVisitor::visitVarAss(string varName, boost::shared_ptr<ExprAST> expr, 
							   boost::shared_ptr<SymbolTable> st) {
	boost::tuple< string, list<AssemCom>, vector<string> > res 
	  = ExprGen::generateExpression(expr->getRoot(), st, _freeRegs);
	string rhs = res.get<0>();
	list<AssemCom> exprInstrs = res.get<1>();
	_freeRegs = res.get<2>();

	_instrs.splice(_instrs.end(), exprInstrs);

	boost::shared_ptr<Identifier> varIdent 
	  = st->lookupCurrLevelAndEnclosingLevels(varName);
	boost::shared_ptr<Variable> var 
	  = boost::shared_polymorphic_downcast<Variable>(varIdent);

	string loc = var->getAssLoc();

	if (loc == "") {
		if (_freeRegs.empty()) {
			// TODO: memory allocation
			loc = "TODO";
		} else {
			loc = _freeRegs.front();
			_freeRegs.erase(_freeRegs.begin());
		}
	}

	// mov loc, rhs
	vector<string> args;
	args.push_back(loc);
	args.push_back(rhs);
	AssemCom mov("mov", args.size(), args);
	_instrs.push_back(mov);
}

void ASTVisitor::visitFuncCall(string name,
						    	 boost::shared_ptr<CallParamsAST> params, 
							     boost::shared_ptr<SymbolTable> st) {


	vector<boost::shared_ptr< ExprAST> > exprs = params->getParamExprs();

	vector<boost::shared_ptr< ExprAST> >::iterator it;
	int i = 0;

	for (it = exprs.begin(); it != exprs.end(); ++it) {
		pANTLR3_BASE_TREE cp = (*it)->getRoot();

		boost::tuple< string, list<AssemCom>, vector<string> > genParam
	  	  = ExprGen::generateExpression(cp, st, _freeRegs);

	  	string paramLoc = genParam.get<0>();
	  	list<AssemCom> pInstrs = genParam.get<1>();
		_freeRegs = genParam.get<2>();
	  	_instrs.splice(_instrs.end(), pInstrs);

	  	if (i < 4) {
			std::ostringstream s;
			s << "r" << i;
	  		if (paramLoc != s.str()) {
	  			//Parameter needs to be moved into correct register
	  			vector<string> args;
				args.push_back(s.str());
				args.push_back(paramLoc);
				AssemCom mov("mov", args.size(), args);
				_instrs.push_back(mov);
	  		}
	  	} else {
	  		// Push any other params
			if (paramLoc[0] == 'r') {
				vector<string> args;
				args.push_back("{" + paramLoc + "}");
				AssemCom push("push", args.size(), args);
				_instrs.push_back(push);										// push {ri}
			} else if (_freeRegs.empty()) {
				// Need to temporarily borrow a register
				vector<string> args;
				args.push_back("{r0}");
				AssemCom push("push", args.size(), args);
				_instrs.push_back(push);										// push {r0}

				args.clear();
				args.push_back("r0");
				args.push_back(paramLoc);
				AssemCom mov("mov", args.size(), args);
				_instrs.push_back(mov);											// mov r0 ri

				_instrs.push_back(push);										// push {r0}

				args.clear();
				args.push_back("{r0}");
				AssemCom pop("pop", args.size(), args);
				_instrs.push_back(pop);											// pop {ro}
			} else {
				string reg = _freeRegs.front();

				vector<string> args;
				args.push_back(reg);
				args.push_back(paramLoc);
				AssemCom mov("mov", args.size(), args);
				_instrs.push_back(mov);											// mov reg paramLoc

				args.clear();
				args.push_back("{" + reg + "}");
				AssemCom push("push", args.size(), args);
				_instrs.push_back(push);										// push reg
			}
	  	}
	  	i++;
	}

	vector<string> blArgs;
	blArgs.push_back(name);
	_instrs.push_back(AssemCom("bl", 1, blArgs));
}

void ASTVisitor::visitArrayAssign(string name,
                  					boost::shared_ptr<ExprAST> index,
                  					boost::shared_ptr<ExprAST> value, 
							        boost::shared_ptr<SymbolTable> st) {}

void ASTVisitor::visitArrayDec(string name, boost::shared_ptr<ExprAST> length, 
								 boost::shared_ptr<Type> type, 
							     boost::shared_ptr<SymbolTable> st) {
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
			_instrs.push_back(AssemCom(".comm", 2, comm));
			Label l;
			_instrs.push_back(AssemCom(l.getLabel() + ":", 0,
											std::vector<string>()));
			std::vector<string> word;
			word.push_back(name);
			_instrs.push_back(AssemCom(".word", 1, word));
			arr->setAssLoc(l.getLabel());																	
		} else if (arrType->getTypeName() == "Letter") {
			std::vector<string> comm;
			comm.push_back(name);
			comm.push_back("1*something"); 										//need to access array length here.
			_instrs.push_back(AssemCom(".comm", 2, comm));
			Label l;
			_instrs.push_back(AssemCom(l.getLabel() + ":", 0, 
											std::vector<string>()));
			std::vector<string> word;
			word.push_back(name);
			_instrs.push_back(AssemCom(".word", 1, word));	
			arr->setAssLoc(l.getLabel());
		} else {
			// TODO: string case
		}
	}
}

list<AssemCom> ASTVisitor::getInstrs() {
	return _instrs;
}
