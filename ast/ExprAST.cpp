#include "ExprAST.hpp"
#include "CallParamsAST.hpp"
#include "FuncAST.hpp"
#include "../idents/Array.hpp"
#include "../idents/Boolean.hpp"
#include "../idents/Callable.hpp"
#include "../idents/Function.hpp"
#include "../idents/Letter.hpp"
#include "../idents/Number.hpp"
#include "../idents/Sentence.hpp"
#include "../idents/Variable.hpp"
#include "../Utils.hpp"

ExprAST::ExprAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree,
                   boost::shared_ptr<ASTNode> parent, int lineNo,
                   bool hasExprTok) : ASTNode(st, parent, lineNo) {
    _boolArgBoolRet.insert("!");

    _intArgIntRet.insert("~");
    _intArgIntRet.insert("+");
    _intArgIntRet.insert("-");

    _boolArgsBoolRet.insert("||");
    _boolArgsBoolRet.insert("&&");

    // Mixed: number or letter (LHS and RHS must match)
    _mixedArgsMixedRet.insert("|");
    _mixedArgsMixedRet.insert("^");
    _mixedArgsMixedRet.insert("&");
    _mixedArgsMixedRet.insert("&");
    _mixedArgsMixedRet.insert("&");
    _mixedArgsMixedRet.insert("+");
    _mixedArgsMixedRet.insert("-");
    _mixedArgsMixedRet.insert("*");
    _mixedArgsMixedRet.insert("/");
    _mixedArgsMixedRet.insert("%");

    _mixedArgsBoolRet.insert("==");
    _mixedArgsBoolRet.insert("!=");
    _mixedArgsBoolRet.insert(">");
    _mixedArgsBoolRet.insert("<");
    _mixedArgsBoolRet.insert(">=");
    _mixedArgsBoolRet.insert("<=");

    _type = boost::shared_ptr<Type>();
    _tree = tree;
    _lineNo = lineNo;
    _isDeclarable = false;
	_hasExprTok = hasExprTok;
    check();
}

void ExprAST::check() {
    int children = _tree->getChildCount(_tree);
    assert (children > 0);

    pANTLR3_BASE_TREE root = (_hasExprTok) ?
      Utils::childByNum(_tree, 0) : _tree;
    string tok = Utils::createStringFromTree(root);

    if (tok == "FUNC") {
        // Inline function call, evaluates to function's return type

        string funcName = Utils::createStringFromTree(Utils::childByNum(root,
                                                                          0));
        pANTLR3_BASE_TREE cplTree = Utils::childByNum(root, 1);

        // Check that function call and parameters type-check
        //   i.e. function name in scope, and parameters exprs match expected
        //        type
        boost::shared_ptr<CallParamsAST> callParamsNode(
          new CallParamsAST(_st, cplTree, _parent, _lineNo)
        );
        FuncAST funcCheck(_st, funcName, callParamsNode, _parent, _lineNo);

        boost::shared_ptr<Identifier> funcIdent =
          _st->lookupCurrLevelAndEnclosingLevels(funcName);

		// Don't need to output an error if the below if fails, as
		//   CallParamsAST or FuncAST will have done it for me
		if (funcIdent && funcIdent->getBaseName() == "Callable") {
            boost::shared_ptr<Callable> funcCallable =
              boost::shared_polymorphic_downcast<Callable>(funcIdent);

            if (funcCallable->getCallableFuncOrProc() != "Function") {
				Utils::printSemErr(_lineNo, (string) "Can't use procedure " +
				                     funcName + " in an expression since it " +
				                     "has no return value.");
            } else {
                boost::shared_ptr<Function> func =
                  boost::shared_polymorphic_downcast<Function>(funcIdent);
                _type = func->getType();
            }
		}
    } else if (tok == "VAR") {
        // Variable reference, evaluates to variable's type
        // Also allowed to be an array, so that function calls with array
        //   arguments are allowed

        string varName = Utils::createStringFromTree(Utils::childByNum(root,
                                                                         0));

        boost::shared_ptr<Identifier> varIdent =
          _st->lookupCurrLevelAndEnclosingLevels(varName);

        if (!varIdent) {
			Utils::printSemErr(_lineNo, varName + " is not in scope!");
        } else {
            string baseName = varIdent->getBaseName();

            if (baseName == "Type") {
                boost::shared_ptr<Type> varType =
                  boost::shared_polymorphic_downcast<Type>(varIdent);

                if (varType->getTypeName() != "Array") {
					Utils::printSemErr(_lineNo, varName + " is not in scope!");
                } else {
                    boost::shared_ptr<Array> arr =
                      boost::shared_polymorphic_downcast<Array>(varType);
                    _type = arr;
                }
            } else if (baseName == "Variable") {
                boost::shared_ptr<Variable> var =
                  boost::shared_polymorphic_downcast<Variable>(varIdent);
                _isDeclarable = true;
                _type = var->getTypeName();
            } else {
              	Utils::printSemErr(_lineNo, varName + " is not a variable!");
            }
        }
    } else if (tok == "ARRMEMBER") {
        // Array member reference, evaluates to array's element type

        string arrName = Utils::createStringFromTree(Utils::childByNum(root,
                                                                         0));

        boost::shared_ptr<Identifier> arrIdent =
          _st->lookupCurrLevelAndEnclosingLevels(arrName);

        if (!arrIdent) {
	        Utils::printSemErr(_lineNo, arrName + " is not in scope!");
        } else if (arrIdent->getBaseName() != "Type") {
			Utils::printSemErr(_lineNo, arrName + " is not an array.");
        } else {
            boost::shared_ptr<Type> arrType =
              boost::shared_polymorphic_downcast<Type>(arrIdent);

            if (arrType->getTypeName() != "Array") {
                Utils::printSemErr(_lineNo, arrName + " is not an array.");
            } else {
                boost::shared_ptr<Array> arr =
                  boost::shared_polymorphic_downcast<Array>(arrIdent);

                pANTLR3_BASE_TREE index = Utils::childByNum(root, 1);
                ExprAST indexCheck(_st, index, _parent, _lineNo, true);

                if (indexCheck.getType()) {
					string indexType = indexCheck.getType()->getTypeName();
                    if (indexType != "Number") {
						Utils::printSemErr(_lineNo, (string) "Array index " +
						                     "must evaluate to a Number" +
						                     ", not a " + indexType);
                    } else {
                        _isDeclarable = true;
                        _type = arr->getElemType();
                    }
                }
            }
        }
        
    } else if (tok == "'") {
        // Char of form 'x', evaluates to a Letter

        string let = Utils::createStringFromTree(Utils::childByNum(root, 0));
        if (let.length() != 1) {
			Utils::printSynErr(_lineNo, (string) "Expected Letter inside " +
                                   "single quotes, got a Sentence.");
        } else {
            boost::shared_ptr<Type> letter(new Letter);
            _type = letter;
        }
    } else if (tok == "\"") {
        // String of form "foo", evaluates to a Sentence

        boost::shared_ptr<Type> sentence(new Sentence);
        _type = sentence;
    } else {
        // Recursive case, will resolve to a Boolean or Number

        boost::shared_ptr<Type> retType = recurseTree(root, "*");
        _type = retType;
    }
}

boost::shared_ptr<Type> ExprAST::recurseTree(pANTLR3_BASE_TREE tree,
                                               string expectedType) {
    int children = tree->getChildCount(tree);
    assert (0 <= children && children < 3);

    if (children == 0) {
        // Number base case

        boost::shared_ptr<Type> number(new Number());

        if (expectedType != "*" && expectedType != "Number") {
			Utils::printSemErr(_lineNo, "Expected expression to evaluate to " +
			                     expectedType + ", but got a Number.");
            return boost::shared_ptr<Type>();
        }

        return number;
    } else if (children == 1) {
        // Unary operator

        string op = Utils::createStringFromTree(tree);
        pANTLR3_BASE_TREE arg = Utils::childByNum(tree, 0);

        string expEvalType;
        boost::shared_ptr<Type> evaluatedType;
        
        if (_intArgIntRet.find(op) != _intArgIntRet.end()) {
            expEvalType = "Number";
        } else if (_boolArgBoolRet.find(op) != _boolArgBoolRet.end()) {
            expEvalType = "Boolean";
        } else if (op == "'") {
            // Letter base case

            string let = Utils::createStringFromTree(arg);
            if (let.length() != 1) {
				Utils::printSynErr(_lineNo, (string) "Expected Letter " +
				                    "inside single quotes, got a Sentence.");
                return boost::shared_ptr<Type>();
            }

            if (expectedType != "*" && expectedType != "Letter") {
                Utils::printSemErr(_lineNo, "'" + let + "' is a Letter, not " +
                                     "a " + expectedType + ".");
                return boost::shared_ptr<Type>();
            }

            boost::shared_ptr<Type> letter(new Letter);
            return letter;
        } else if (op == "\"") {
            // String base case, error case

            Utils::printSemErr(_lineNo, (string) "Invalid usage of Sentence " + 
                                 "as operator argument.");
            return boost::shared_ptr<Type>();
        } else {
			// Note the false flag because EXPR root token has been stripped
            ExprAST checkExp(_st, tree, _parent, _lineNo, false);
            evaluatedType = checkExp.getType();

            if (!evaluatedType) {
                // Error has already been outputted, we just need to break out
                //   of the parent call too
                return boost::shared_ptr<Type>();
            }

            if (evaluatedType->getTypeName() == "Array") {
                boost::shared_ptr<Array> evaluatedTypeArray =
                  boost::shared_polymorphic_downcast<Array>(evaluatedType);
                evaluatedType = evaluatedTypeArray->getElemType();
            }

            if (!evaluatedType) {
                // Error has already been outputted, we just need to break out of
                //   the parent call too
                return boost::shared_ptr<Type>();
            }

            if (expectedType != "*" && expectedType != evaluatedType->getTypeName()) {
                cerr << "Line " << _lineNo << " - " << "Expected argument expression to " << op << " to evaluate to a " << expectedType << ", but got a " << evaluatedType->getTypeName() << endl;
                return boost::shared_ptr<Type>();
			}

            return evaluatedType;
        }

        evaluatedType = recurseTree(arg, expEvalType);

        if (!evaluatedType) {
            // Error has already been outputted, we just need to break out of
            //   the parent call too
            return boost::shared_ptr<Type>();
        }

        if (evaluatedType->getTypeName() == "Array") {
            boost::shared_ptr<Array> evaluatedTypeArray =
              boost::shared_polymorphic_downcast<Array>(evaluatedType);
            evaluatedType = evaluatedTypeArray->getElemType();
        }

        if (!evaluatedType) {
            // Error has already been outputted, we just need to break out of
            //   the parent call too
            return boost::shared_ptr<Type>();
        }

        if (expectedType != "*" && expectedType !=
              evaluatedType->getTypeName()) {
            Utils::printSemErr(_lineNo, "Expected argument to " + op + "to " +
                                 "be a " + expectedType + ", not a " +
                                  evaluatedType->getTypeName() + ".");
           return boost::shared_ptr<Type>();
        }
        
        return evaluatedType;
    } else if (children == 2) {
        // Binary operator
        string op = Utils::createStringFromTree(tree);
        pANTLR3_BASE_TREE lhs = Utils::childByNum(tree, 0);
        pANTLR3_BASE_TREE rhs = Utils::childByNum(tree, 1);

        string lhsTok = Utils::createStringFromTree(lhs);
        string rhsTok = Utils::createStringFromTree(rhs);

        boost::shared_ptr<Type> lhsType, rhsType;

        if (_mixedArgsMixedRet.find(op) != _mixedArgsMixedRet.end()) {
            // Args must be of same type
            // Return type is type of either arg

            lhsType = recurseTree(lhs, "*");
            rhsType = recurseTree(rhs, "*");

            if (!lhsType || !rhsType) {
                // Error has already been outputted, we just need to break out
                // of the parent call too
                return boost::shared_ptr<Type>();
            }

            if (lhsType->getTypeName() == "Array") {
                boost::shared_ptr<Array> lhsTypeArray =
                  boost::shared_polymorphic_downcast<Array>(lhsType);
                lhsType = lhsTypeArray->getElemType();
            }

            if (rhsType->getTypeName() == "Array") {
                boost::shared_ptr<Array> rhsTypeArray =
                  boost::shared_polymorphic_downcast<Array>(rhsType);
                rhsType = rhsTypeArray->getElemType();
            }

            if (!lhsType || !rhsType) {
                // Error has already been outputted, we just need to break out
                //   of the parent call too
                return boost::shared_ptr<Type>();
            }

            string lhsTypeStr = lhsType->getTypeName();
            string rhsTypeStr = rhsType->getTypeName();
            if (lhsTypeStr != rhsTypeStr) {
                Utils::printSemErr(_lineNo, "Operand type mismatch for " + op +
                                     " (" + lhsTypeStr + " does not match " +
                                     rhsTypeStr + ").");
                return boost::shared_ptr<Type>();
            } else if (expectedType != "*" &&
                         lhsType->getTypeName() != expectedType) {
                Utils::printSemErr(_lineNo, "Operator " + op + " will return " +
                                     "a " + lhsType->getTypeName() + "not a " +
                                     expectedType + ".");
                return boost::shared_ptr<Type>();
            }
            return lhsType;
        } else if (_mixedArgsBoolRet.find(op) != _mixedArgsBoolRet.end()) {
            // Args must be of same type
            // Boolean return type

            if (expectedType != "*" && expectedType != "Boolean") {
                // No point even evaluating lhs or rhs because my return type is
                //   wrong
                Utils::printSemErr(_lineNo, "Operator " + op + " returns a " +
                                     "Boolean, not a " + expectedType + ".");
                return boost::shared_ptr<Type>();
            } else {
				lhsType = recurseTree(lhs, "*");
	            rhsType = recurseTree(rhs, "*");

                if (!lhsType || !rhsType) {
                    // Error has already been outputted, we just need to break
                    //   out of the parent call too
                    return boost::shared_ptr<Type>();
                }

                if (lhsType->getTypeName() == "Array") {
                    boost::shared_ptr<Array> lhsTypeArray =
                      boost::shared_polymorphic_downcast<Array>(lhsType);
                    lhsType = lhsTypeArray->getElemType();
                }

                if (rhsType->getTypeName() == "Array") {
                    boost::shared_ptr<Array> rhsTypeArray =
                      boost::shared_polymorphic_downcast<Array>(rhsType);
                    rhsType = rhsTypeArray->getElemType();
                }

                if (!lhsType || !rhsType) {
                    // Error has already been outputted, we just need to break
                    //   out of the parent call too
                    return boost::shared_ptr<Type>();
                }

                string lhsTypeStr = lhsType->getTypeName();
                string rhsTypeStr = rhsType->getTypeName();
                if (lhsTypeStr != rhsTypeStr) {
                    Utils::printSemErr(_lineNo, "Operand type mismatch for "
                                         + op + " (" + lhsTypeStr +
                                         " does not match " + rhsTypeStr +
                                         ").");
                    return boost::shared_ptr<Type>();
                }

                boost::shared_ptr<Type> evaluatedType(new Boolean);
                return evaluatedType;
            }
        } else if (_boolArgsBoolRet.find(op) != _boolArgsBoolRet.end()) {
            // Args must be boolean
            // Boolean return type

            if (expectedType != "*" && expectedType != "Boolean") {
                // No point even evaluating lhs or rhs because my return type is
                //   wrong
                Utils::printSemErr(_lineNo, "Operator " + op + " returns a " +
                                     "Boolean, not a " + expectedType + ".");
                return boost::shared_ptr<Type>();
            } else {
                lhsType = recurseTree(lhs, "Boolean");
                rhsType = recurseTree(rhs, "Boolean");

                if (!lhsType || !rhsType) {
                    // Error has already been outputted, we just need to break
                    //   out of the parent call too
                    return boost::shared_ptr<Type>();
                }

                boost::shared_ptr<Type> evaluatedType(new Boolean);
                return evaluatedType;
            }
        } else {
            // Note the false flag because EXPR root token has been stripped
            ExprAST checkExp(_st, tree, _parent, _lineNo, false);
            boost::shared_ptr<Type> evaluatedType = checkExp.getType();

            if (!evaluatedType) {
                // Error has already been outputted, we just need to break out
                //   of the parent call too
                return boost::shared_ptr<Type>();
            }

            if (evaluatedType->getTypeName() == "Array") {
                boost::shared_ptr<Array> evaluatedTypeArray =
                  boost::shared_polymorphic_downcast<Array>(evaluatedType);
                evaluatedType = evaluatedTypeArray->getElemType();
            }

            if (!evaluatedType) {
                // Error has already been outputted, we just need to break out
                //   of the parent call too
                return boost::shared_ptr<Type>();
            }

            if (expectedType != "*" && expectedType !=
                  evaluatedType->getTypeName()) {
                Utils::printSemErr(_lineNo, (string) "Expected argument " +
                                     "expression evaluate to a " +
                                     expectedType + ", but" + "got a " +
                                     evaluatedType->getTypeName());
                return boost::shared_ptr<Type>();
            }
            
            return evaluatedType;
        }
    }
}

boost::shared_ptr<Type> ExprAST::getType() {
    return _type;
}

bool ExprAST::isDeclarable() {
    return _isDeclarable;
}
