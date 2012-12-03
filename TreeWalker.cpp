#include "TreeWalker.hpp"
#include "Utils.hpp"
#include "ast/VarDecAST.hpp"
#include "ast/ExprAST.hpp"
#include "ast/ArrayDecAST.hpp"
#include "ast/ArrayAssignAST.hpp"
#include "ast/VarAssignAST.hpp"
#include "ast/FuncProcCallAST.hpp"
#include "ast/CallParamsAST.hpp"
#include "ast/HeaderParamsAST.hpp"
#include "ast/ProcDecAST.hpp"
#include "ast/FuncDecAST.hpp"
#include "ast/IncAST.hpp"
#include "ast/DecAST.hpp"
#include "ast/PrintAST.hpp"
#include "ast/ReturnAST.hpp"
#include "ast/StdinAST.hpp"
#include "ast/WhileAST.hpp"
#include "ast/ChoiceAST.hpp"
#include "ast/IfAST.hpp"
#include "ast/IfBodyAST.hpp"
#include "ast/ProgAST.hpp"
#include <antlr3commontree.h>

//Construct object, assign fields and start walking the input tree from the top
TreeWalker::TreeWalker(boost::shared_ptr<SymbolTable> topSt, 
                         pANTLR3_BASE_TREE inputTree, 
                         boost::shared_ptr<AST> outputTree) {
    _topSt = topSt;
    _inputTree = inputTree;
    _outputTree = outputTree;

    init();
    walk(_inputTree, _topSt, boost::shared_ptr<ASTNode>(), 0);

    checkHatta();
}

// Call the correct method based on the type of the current AST node
void TreeWalker::walk(pANTLR3_BASE_TREE tree, boost::shared_ptr<SymbolTable> st, 
                        boost::weak_ptr<ASTNode> parent, 
                        int childNum) {
    string tokenName = Utils::createStringFromTree(tree);
    //In header
    // typedef void (TreeWalker::*PROC)(pANTLR3_BASE_TREE, 
    //                                    boost::shared_ptr<SymbolTable>, 
    //                                    boost::weak_ptr<ASTNode>, int);
    map<string, PROC>::const_iterator it = _memberMap.find(tokenName);
    if (it != _memberMap.end()) {
        PROC tokenProcessor = it->second;
        (this->*tokenProcessor)(tree, st, parent, childNum);
    }
}

// Create the root of the tree and first non-static symbol table, go to children
void TreeWalker::processPROG(pANTLR3_BASE_TREE tree, 
                               boost::shared_ptr<SymbolTable> st, 
                               boost::weak_ptr<ASTNode> parent, int childNum) {
    boost::shared_ptr<SymbolTable> progSt(new SymbolTable(st));
    boost::weak_ptr<SymbolTable> p(progSt);
    st->addChild(p);

    boost::shared_ptr<ProgAST> root(new ProgAST(progSt, 
                                                boost::shared_ptr<ASTNode>(), 
                                                getLine(tree)));
    boost::weak_ptr<ProgAST> r(root);
    _outputTree->setRoot(root);

    for (int i = 0; i < tree->getChildCount(tree); ++i) {
        walk(Utils::childByNum(tree, i), progSt, r, i);
    }
}

// Create the param list, get the id, create node and walk to body
void TreeWalker::processPROCDEC(pANTLR3_BASE_TREE tree, 
                                  boost::shared_ptr<SymbolTable> st, 
                                  boost::weak_ptr<ASTNode> parent, 
                                  int childNum) {
    boost::shared_ptr<HeaderParamsAST> params(
      new HeaderParamsAST(st, Utils::childByNum(tree, 1), parent, 
                            getLine(tree)));

    pANTLR3_BASE_TREE idTree = Utils::childByNum(tree, 0);
    string procName = Utils::createStringFromTree(idTree);

    boost::shared_ptr<SymbolTable> scopeSt(new SymbolTable(st));
    boost::weak_ptr<SymbolTable> s(scopeSt);
    st->addChild(s);

    boost::shared_ptr<ProcDecAST> dec(new ProcDecAST(scopeSt, procName, params, 
                                                       parent, getLine(tree)));
    boost::weak_ptr<ProcDecAST> d(dec);
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(dec);
    } else {
        cerr << "Bad parent of " << procName << "." << endl;
    }
    walk(Utils::childByNum(tree, 2), scopeSt, d, 2);
}

// We don't need a AST node here, but we do need a new st as we have a new scope

// If we could come up with a way to avoid having a new SymbolTable here and in 
// the levels directly above then that would be good
void TreeWalker::processBODY(pANTLR3_BASE_TREE tree, 
                               boost::shared_ptr<SymbolTable> st, 
                               boost::weak_ptr<ASTNode> parent, int childNum) {

    boost::shared_ptr<SymbolTable> scopeSt(new SymbolTable(st));
    boost::weak_ptr<SymbolTable> s(scopeSt);
    st->addChild(s);

    for (int i = 0; i < tree->getChildCount(tree); ++i) {
        walk(Utils::childByNum(tree, i), scopeSt, parent, i);
    }
}

// Like PROCDEC but with a type
void TreeWalker::processFUNCDEC(pANTLR3_BASE_TREE tree, 
                                  boost::shared_ptr<SymbolTable> st, 
                                  boost::weak_ptr<ASTNode> parent, 
                                  int childNum) {
    boost::shared_ptr<HeaderParamsAST> params(
      new HeaderParamsAST(st, Utils::childByNum(tree, 1), parent, 
                            getLine(tree)));

    pANTLR3_BASE_TREE idTree = Utils::childByNum(tree, 0);
    pANTLR3_BASE_TREE typeTree = Utils::childByNum(tree, 2);

    string funcName = Utils::createStringFromTree(idTree);
    string funcType = Utils::createStringFromTree(typeTree);

    boost::shared_ptr<SymbolTable> scopeSt(new SymbolTable(st));
    boost::weak_ptr<SymbolTable> s(scopeSt);
    st->addChild(s);

    boost::shared_ptr<FuncDecAST> dec(
      new FuncDecAST(scopeSt, funcName, params, funcType, parent, 
                       getLine(tree)));
    boost::weak_ptr<FuncDecAST> d (dec);
    pANTLR3_BASE_TREE bodyTree = Utils::childByNum(tree, 3);

    if (!findReturn(bodyTree)) {
        Utils::printSemErr(getLine(tree), "Function " + funcName + 
                            " does not have a return statement for all paths.");
    }
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(dec);
    } else {
        cerr << "Bad parent of " << funcName << "." << endl;
    }
    walk(Utils::childByNum(tree, 3), scopeSt, d, 3);
}

// Let's check if a function definition has a return statement
bool TreeWalker::findReturn(pANTLR3_BASE_TREE tree) {
    bool thislevelflag = false;
    for (int i = 0; i < tree->getChildCount(tree); ++i) {
        string tok = Utils::createStringFromTree(Utils::childByNum(tree, i));

        if (tok == "IF" | tok == "WHILE" | tok == "CHOICE" | tok == "IFSTS" | 
              tok == "ELSESTS") {
            thislevelflag = findReturn(Utils::childByNum(tree, i));
        }

        if (tok == "RETURN") {
            thislevelflag = true;
        }
    }

    return thislevelflag;
}

// Work out if it's an array or not, then process accordingly
void TreeWalker::processVARDEC(pANTLR3_BASE_TREE tree, 
                                 boost::shared_ptr<SymbolTable> st, 
                                 boost::weak_ptr<ASTNode> parent, 
                                 int childNum) {
    pANTLR3_BASE_TREE idTree = Utils::childByNum(tree, 0);
    string varName = Utils::createStringFromTree(idTree);

    pANTLR3_BASE_TREE varOptionsTree = Utils::childByNum(tree, 1);
    string typeName = 
      Utils::createStringFromTree(Utils::childByNum(varOptionsTree, 0));

    pANTLR3_BASE_TREE exprTree = Utils::childByNum(varOptionsTree, 1);
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        if (Utils::createStringFromTree(varOptionsTree) == "NEWVAR") {
            if (exprTree == NULL) {
                boost::shared_ptr<VarDecAST> dec(
                  new VarDecAST(st, typeName, varName, parent, getLine(tree)));
                p->addChild(dec);
            } else {
                boost::shared_ptr<VarDecAST> dec(
                  new VarDecAST(st, typeName, varName, parent, getLine(tree)));

                boost::shared_ptr<ExprAST> expr(
                  new ExprAST(st, exprTree, parent, getLine(tree), true));
                boost::shared_ptr<VarAssignAST> assign(
                  new VarAssignAST(st, varName, expr, parent, getLine(tree)));

                p->addChild(dec);
                p->addChild(assign);
            }
        } else {
            boost::shared_ptr<ExprAST> expr(
              new ExprAST(st, exprTree, parent, getLine(tree), true));

            boost::shared_ptr<ArrayDecAST> dec(
              new ArrayDecAST(st, expr, varName, typeName, parent, 
                                getLine(tree)));

            dec->addChild(expr);
            p->addChild(dec);
        }
    } else {
        cerr << "Bad parent." << endl;
    }
}

void TreeWalker::processVARSTAT(pANTLR3_BASE_TREE tree, 
                                  boost::shared_ptr<SymbolTable> st, 
                                  boost::weak_ptr<ASTNode> parent, 
                                  int childNum) {
    string varId = Utils::createStringFromTree(Utils::childByNum(tree, 0));

    pANTLR3_BASE_TREE optionsTree = Utils::childByNum(tree, 1);
    string option = Utils::createStringFromTree(optionsTree);
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        if (option == "ARRMEMBER") {
            boost::shared_ptr<ExprAST> elem(
              new ExprAST(st, Utils::childByNum(optionsTree, 0), parent, 
                            getLine(tree), true));

            boost::shared_ptr<ExprAST> val(
              new ExprAST(st, Utils::childByNum(optionsTree, 1), parent, 
                            getLine(tree), true));

            boost::shared_ptr<ArrayAssignAST> assign(
              new ArrayAssignAST(st, varId, elem, val, parent, getLine(tree)));
            p->addChild(assign);
        } else if (option == "FUNC") {
            boost::shared_ptr<CallParamsAST> params(
              new CallParamsAST(st, Utils::childByNum(optionsTree, 0), parent, 
                                  getLine(tree)));

            boost::shared_ptr<FuncProcCallAST> func(
              new FuncProcCallAST(st, varId, params, parent, getLine(tree)));

            p->addChild(func);
        } else if (option == "ASSIGN") {
            boost::shared_ptr<ExprAST> expr(
              new ExprAST(st, Utils::childByNum(optionsTree, 0), parent, 
                            getLine(tree), true));

            boost::shared_ptr<VarAssignAST> assign(
              new VarAssignAST(st, varId, expr, parent, getLine(tree)));

            p->addChild(assign);
        }
    } else {
        cout << "Bad parent." << endl;
    }
}

void TreeWalker::processINC(pANTLR3_BASE_TREE tree, 
                              boost::shared_ptr<SymbolTable> st, 
                              boost::weak_ptr<ASTNode> parent, 
                              int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));
    boost::shared_ptr<IncAST> inc(new IncAST(st, expr, parent, getLine(tree)));
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(inc);
    } else {
        cout << "Bad parent." << endl;
    }
}


void TreeWalker::processDEC(pANTLR3_BASE_TREE tree, 
                              boost::shared_ptr<SymbolTable> st, 
                              boost::weak_ptr<ASTNode> parent, int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));

    boost::shared_ptr<DecAST> dec(new DecAST(st, expr, parent, getLine(tree)));
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(dec);
    } else {
        cout << "Bad parent." << endl;
    }
}

void TreeWalker::processPRINT(pANTLR3_BASE_TREE tree, 
                                boost::shared_ptr<SymbolTable> st, 
                                boost::weak_ptr<ASTNode> parent, int childNum) {
    pANTLR3_BASE_TREE optionTree = Utils::childByNum(tree, 0);

    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, optionTree, parent, getLine(tree), true));

    boost::shared_ptr<PrintAST> print(
      new PrintAST(st, expr, parent, getLine(tree)));

    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(print);
    } else {
        cout << "Bad parent." << endl;
    }
}

void TreeWalker::processRETURN(pANTLR3_BASE_TREE tree, 
                                 boost::shared_ptr<SymbolTable> st, 
                                 boost::weak_ptr<ASTNode> parent, 
                                 int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));

    boost::shared_ptr<ReturnAST> ret(
        new ReturnAST(st, expr, parent, getLine(tree)));

    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(ret);
    } else {
        cout << "Bad parent." << endl;
    }
}

void TreeWalker::processSTDIN(pANTLR3_BASE_TREE tree, 
                                boost::shared_ptr<SymbolTable> st, 
                                boost::weak_ptr<ASTNode> parent, int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));

    boost::shared_ptr<StdinAST> in(
      new StdinAST(st, expr, parent, getLine(tree)));
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(in);
    } else {
        cout << "Bad parent." << endl;
    }
}

void TreeWalker::processWHILE(pANTLR3_BASE_TREE tree, 
                                boost::shared_ptr<SymbolTable> st, 
                                boost::weak_ptr<ASTNode> parent, int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));

    boost::shared_ptr<WhileAST> whilenode(
      new WhileAST(st, expr, parent, getLine(tree)));

    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(whilenode);
    } else {
        cout << "Bad parent." << endl;
    }

    for (int i = 1; i < tree->getChildCount(tree); ++i) {
        walk(Utils::childByNum(tree, i), st, whilenode, i - 1);
    }
}

void TreeWalker::processCHOICE(pANTLR3_BASE_TREE tree, 
                                 boost::shared_ptr<SymbolTable> st, 
                                 boost::weak_ptr<ASTNode> parent, 
                                 int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));

    boost::shared_ptr<IfBodyAST> ifs(new IfBodyAST(st, parent, getLine(tree)));

    boost::shared_ptr<IfBodyAST> elses(new IfBodyAST(st, parent, getLine(tree)));

    boost::shared_ptr<ChoiceAST> choice(
      new ChoiceAST(st, expr, ifs, elses, parent, getLine(tree)));

    choice->addChild(ifs);
    choice->addChild(elses);
    
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(choice);
    } else {
        cout << "Bad parent." << endl;
    }

    pANTLR3_BASE_TREE ifTree = Utils::childByNum(tree, 1);
    pANTLR3_BASE_TREE elseTree = Utils::childByNum(tree, 2);

    for (int i = 0; i < ifTree->getChildCount(ifTree); ++i) {
        walk(Utils::childByNum(ifTree, i), st, ifs, i);
    }

    for (int i = 0; i < elseTree->getChildCount(elseTree); ++i) {
        walk(Utils::childByNum(elseTree, i), st, elses, i);
    }
}

void TreeWalker::processIF(pANTLR3_BASE_TREE tree, 
                             boost::shared_ptr<SymbolTable> st, 
                             boost::weak_ptr<ASTNode> parent, int childNum) {
    boost::shared_ptr<ExprAST> expr(
      new ExprAST(st, Utils::childByNum(tree, 0), parent, getLine(tree), true));

    boost::shared_ptr<IfBodyAST> body(new IfBodyAST(st, parent, getLine(tree)));

    boost::shared_ptr<IfAST> ifnode(
      new IfAST(st, expr, body, parent, getLine(tree)));

    ifnode->addChild(body);

    pANTLR3_BASE_TREE ifTree = Utils::childByNum(tree, 1);

    for (int i = 0; i < ifTree->getChildCount(ifTree); ++i) {
        walk(Utils::childByNum(ifTree, i), st, body, i);
    }

    for (int i = 2; i < tree->getChildCount(tree); ++i) {
        pANTLR3_BASE_TREE childTree = Utils::childByNum(tree, i);
        string childName = Utils::createStringFromTree(childTree);
        if (childName == "IF") {
            processIF(childTree, st, parent, i);
        } else {
            boost::shared_ptr<IfBodyAST> elses(
              new IfBodyAST(st, parent, getLine(tree)));

            ifnode->addChild(elses);
            pANTLR3_BASE_TREE elseTree = Utils::childByNum(tree, i);
            for (int i = 0; i < elseTree->getChildCount(elseTree); ++i) {
                walk(Utils::childByNum(elseTree, i), st, elses, i);
            }
        }
    }
    
    if(boost::shared_ptr<ASTNode> p = parent.lock()) {
        p->addChild(ifnode);
    } else {
        cout << "Bad parent." << endl;
    }
}

void TreeWalker::processNS(pANTLR3_BASE_TREE tree,
                             boost::shared_ptr<SymbolTable> st, 
                             boost::weak_ptr<ASTNode> parent, int childNum) {}

int TreeWalker::getLine (pANTLR3_BASE_TREE tree) {
    pANTLR3_COMMON_TREE        cTree;
    pANTLR3_COMMON_TOKEN    token;

    cTree = (pANTLR3_COMMON_TREE)(tree->super);
    token = cTree->token;

    if (token == NULL || token->getLine(token) == 0) {
        if (tree->getChildCount(tree) > 0) {
            pANTLR3_BASE_TREE child;

            child = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
            return child->getLine(child);
        }
        return 0;
    }
    return token->getLine(token);
}

void TreeWalker::checkHatta() {
    boost::weak_ptr<SymbolTable> topFunctionTable = _topSt->getChildren()[0];
    if(boost::shared_ptr<SymbolTable> tFT = topFunctionTable.lock()) {
        
        boost::shared_ptr<Identifier> hattaDef = 
          tFT->lookupCurrLevelOnly("hatta");

        if (hattaDef == boost::shared_ptr<Identifier>()) {
            Utils::printSemErr(0,  "hatta not defined at top level");
        } else if (hattaDef->getBaseName() != "Callable") {
            Utils::printSemErr(0, "hatta is not declared as a procedure");
        } else {
            boost::shared_ptr<Callable> hattaCasted = 
              boost::shared_polymorphic_downcast<Callable>(hattaDef);
            if (hattaCasted->getCallableFuncOrProc() != "Proc") {
                Utils::printSemErr(0, "hatta is not declared as a procedure");
            } else if (!hattaCasted->getParams().empty()) {
                Utils::printSemErr(0, "hatta declared with paramaters");
            }
        }
    } else {
        cerr << "SymbolTable error." << endl;
    }
}

// map intialisation
void TreeWalker::init() {
    _memberMap["PROG"]         = &TreeWalker::processPROG;
    _memberMap["PROCDEC"]   = &TreeWalker::processPROCDEC;
    _memberMap["BODY"]        = &TreeWalker::processBODY;
    _memberMap["FUNCDEC"]   = &TreeWalker::processFUNCDEC;
    _memberMap["VARDEC"]    = &TreeWalker::processVARDEC;
    _memberMap["VARSTAT"]   = &TreeWalker::processVARSTAT;
    _memberMap["INC"]       = &TreeWalker::processINC;
    _memberMap["DEC"]       = &TreeWalker::processDEC;
    _memberMap["PRINT"]     = &TreeWalker::processPRINT;
    _memberMap["RETURN"]    = &TreeWalker::processRETURN;
    _memberMap["STDIN"]     = &TreeWalker::processSTDIN;
    _memberMap["WHILE"]     = &TreeWalker::processWHILE;
    _memberMap["CHOICE"]    = &TreeWalker::processCHOICE;
    _memberMap["IF"]        = &TreeWalker::processIF;
    _memberMap["NS"]        = &TreeWalker::processNS;
}
