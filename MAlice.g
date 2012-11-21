grammar MAlice;

options {
  language = C;
  ASTLabelType = pANTLR3_BASE_TREE;
  output = AST;
}

/* 
  Imaginary tokens used for labelling sub-trees (indented in a vaguely tree-like manner)
*/
tokens {
  PROG;
    PROCDEC;
      BODY;
    FUNCDEC;
      HPL;
    VARDEC;
      NEWVAR;
      NEWARR;
    VARSTAT;
      ARRMEMBER;
      FUNC;
        CPL;
      ASSIGN;
      INC;
      DEC;
    PRINT;
    RETURN;
    STDIN;
    WHILE;
    CHOICE;
    IF;
      COND;
      ELSEIF;
    EXPR;
    VAR;
    NS;
}

@parser::postinclude {
  static void printError(pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8* tokenNames);
}

@parser::apifuncs {
  RECOGNIZER->displayRecognitionError = printError;
}

@parser::members {
    /*
      Overwrite the default error printing method with one written by us
    */
    static void printError(pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8* tokenNames) {
        pANTLR3_PARSER       parser;
        pANTLR3_EXCEPTION    exception;
        pANTLR3_INT_STREAM   stream;
        pANTLR3_COMMON_TOKEN currentToken;
        pANTLR3_STRING       fileName;
        pANTLR3_STRING       tokenString;

        exception = recognizer->state->exception;

        // Print the file name if possible
        if (exception->streamName == NULL) {
            if (((pANTLR3_COMMON_TOKEN)(exception->token))->type == ANTLR3_TOKEN_EOF) {
                ANTLR3_FPRINTF(stderr, "End of input reached: (");
            } else {
                ANTLR3_FPRINTF(stderr, "Unknown file source: (");
            }
        } else {
            fileName = exception->streamName->to8(exception->streamName);
            ANTLR3_FPRINTF(stderr, "\%s(", fileName->chars);
        }

        // Print the line number next
        ANTLR3_FPRINTF(stderr, "\%i) ", exception->line);
        ANTLR3_FPRINTF(stderr, " - error \%i : \%s", exception->type, (pANTLR3_UINT8) exception->message);

        parser       = (pANTLR3_PARSER) recognizer->super;
        stream       = parser->tstream->istream;
        currentToken = (pANTLR3_COMMON_TOKEN) exception->token;
        tokenString  = currentToken->toString(currentToken);

        // Print the token if we can
        ANTLR3_FPRINTF(stderr, ", character \%i.", exception->charPositionInLine);
        if (currentToken != NULL) {
            if (currentToken->type == ANTLR3_TOKEN_EOF) {
                ANTLR3_FPRINTF(stderr, "The error was at <EOF>.");
            } else {
                ANTLR3_FPRINTF(stderr, "\n    Error was near \%s\n    ", 
                    tokenString == NULL ? (pANTLR3_UINT8)"a token without a textual representation." : tokenString->chars);
            }
        }

        // Output an error message corresponding to the error type
        switch (exception->type) {
            case ANTLR3_UNWANTED_TOKEN_EXCEPTION:
                // Spurious input, output expected tokens
                if (tokenNames == NULL) {
                    ANTLR3_FPRINTF(stderr, "This doesn't appear to be valid MAlice...");
                } else {
                    if  (exception->expecting == ANTLR3_TOKEN_EOF) {
                        ANTLR3_FPRINTF(stderr, "This doesn't appear to be valid MAlice - expected <EOF>.\n");
                    }
                    else {
                        ANTLR3_FPRINTF(stderr, "This doesn't appear to be valid MAlice - expected \%s.\n", tokenNames[exception->expecting]);
                    }
                }
                break;

            case ANTLR3_MISSING_TOKEN_EXCEPTION:
                // This would be valid if the token before it was something else
                if  (tokenNames == NULL) {
                    ANTLR3_FPRINTF(stderr, "Missing something before (\%i)...\n", exception->expecting);
                }
                else {
                    if  (exception->expecting == ANTLR3_TOKEN_EOF) {
                        ANTLR3_FPRINTF(stderr, "Missing <EOF>\n");
                    }
                    else {
                        ANTLR3_FPRINTF(stderr, "Missing \%s \n", tokenNames[exception->expecting]);
                    }
                }
                break;

            case ANTLR3_RECOGNITION_EXCEPTION:
                // Basic syntax error - unpredicted token
                ANTLR3_FPRINTF(stderr, "Syntax error...\n");    
                break;

            case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
                // Expecting one token, got a different one
                if  (tokenNames == NULL) {
                    ANTLR3_FPRINTF(stderr, "Syntax error...\n");
                }
                else {
                    if  (exception->expecting == ANTLR3_TOKEN_EOF) {
                        ANTLR3_FPRINTF(stderr, "Expecting <EOF>\n");
                    }
                    else {
                        ANTLR3_FPRINTF(stderr, "Expecting \%s ...\n", tokenNames[exception->expecting]);
                    }
                }
                break;

            case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
                // Reached a dead end in the DFA
                ANTLR3_FPRINTF(stderr, "Cannot match input to any prediction...\n");
                break;

            case ANTLR3_MISMATCHED_SET_EXCEPTION: {
                    ANTLR3_UINT32  count;
                    ANTLR3_UINT32  bit;
                    ANTLR3_UINT32  size;
                    ANTLR3_UINT32  numbits;
                    pANTLR3_BITSET errBits;

                    // We ciykd have dealt with a nubmer of tokens here, but we didn't see any of them
                    ANTLR3_FPRINTF(stderr, "Unexpected input here\n  expected one of: ");

                    // Create set
                    count   = 0;
                    errBits = antlr3BitsetLoad(exception->expectingSet);
                    numbits = errBits->numBits(errBits);
                    size    = errBits->size(errBits);

                    if  (size > 0) {
                        for (bit = 1; bit < numbits && count < size; ++bit) {
                            if  (tokenNames[bit]) {
                                ANTLR3_FPRINTF(stderr, "\%s\%s", count > 0 ? ", " : "", tokenNames[bit]); 
                                count++;
                            }
                        }
                        ANTLR3_FPRINTF(stderr, "\n");
                    }
                    else {
                        ANTLR3_FPRINTF(stderr, "The expected set was actually empty.\n");
                    }
                }
                break;

            case ANTLR3_EARLY_EXIT_EXCEPTION:
                // A token sequence was started, but the end token in the sequence was hit before it should have been
                ANTLR3_FPRINTF(stderr, "This token sequence ended before it should have...\n");
                break;

            default:
                // This shouldn't ever be hit
                ANTLR3_FPRINTF(stderr, "Syntax error...\n");
                break;
        }
    }
}

/*
  PARSER RULES: These rules have non-terminals on the LHS
*/
program: declarationList EOF
         -> ^(PROG declarationList);

declarationList: (declaration)+;

declaration: varDeclaration | funcDeclaration | procDeclaration;

varDeclaration: ID varOptions delimiter
                -> ^(VARDEC ID varOptions);

varOptions: ('was' 'a' type ('too' | 'of' expression)?) -> ^(NEWVAR type expression)| 
            'had' expression type -> ^(NEWARR type expression);

funcDeclaration: 'The' 'room' ID headerParams 'contained' 'a' type body
                 -> ^(FUNCDEC ID headerParams type body);

procDeclaration: 'The' 'looking-glass' ID headerParams body
                 -> ^(PROCDEC ID headerParams body);

headerParams: '(' (headerParamsList)? ')'
              -> ^(HPL headerParamsList?);

headerParamsList: (headerParam) (',' headerParam)*
                  -> headerParam+;

headerParam: (type | refType) ID;

callParams: '(' (callParamsList?) ')'
            -> ^(CPL callParamsList?);

callParamsList: expression (',' expression)*
                -> expression+;

body: 'opened' ((declarationList?) statementList | ) 'closed'
      -> ^(BODY declarationList? statementList);

statementList: (statement)+;

idOptions: APOSTROPHE 's' elem=expression 'piece' 'became' val=expression
           -> ^(ARRMEMBER $elem $val) |

           callParams
           -> ^(FUNC callParams) |

           'became' expression
           -> ^(ASSIGN expression);

print: ('said' 'Alice' | 'spoke');

statement: body |

           '.' -> ^(NS) |

           (expression 'ate') => expression 'ate' delimiter
           -> ^(INC expression) |

           (expression 'drank') => expression 'drank' delimiter
           -> ^(DEC expression) |

           (ID APOSTROPHE 's' expression 'piece' print) => ID APOSTROPHE 's' expression 'piece' print delimiter
           -> ^(PRINT ^(EXPR ^(ARRMEMBER ID expression))) |

           (ID callParams print) => ID callParams print delimiter 
           -> ^(PRINT ^(EXPR ^(FUNC ID callParams))) |

           (ID idOptions) => ID idOptions delimiter 
           -> ^(VARSTAT ID idOptions) |

           expression print delimiter 
           -> ^(PRINT expression) |

           'Alice' 'found' expression '.' 
           -> ^(RETURN expression) |

           ('what' 'was' expression '?' delimiter) => 'what' 'was' expression '?' delimiter
           -> ^(STDIN expression) |

           'what' 'was' expression '?' 
           -> ^(STDIN expression) |

           ('eventually' '(' expression ')' 'because' statementList 'enough' 'times' delimiter) =>
           'eventually' '(' expression ')' 'because' statementList 'enough' 'times' delimiter
           -> ^(WHILE expression statementList) |

           'eventually' '(' expression ')' 'because' statementList 'enough' 'times' 
           -> ^(WHILE expression statementList) |

           ('either' '(' expression ')' 'so' trueS=statementList 'or' falseS=statementList 'because' 'Alice' 'was' 'unsure' 'which' delimiter) =>
           'either' '(' expression ')' 'so' trueS=statementList 'or' falseS=statementList 'because' 'Alice' 'was' 'unsure' 'which' delimiter 
           -> ^(CHOICE expression $trueS $falseS) |

           'either' '(' expression ')' 'so' trueS=statementList 'or' falseS=statementList 'because' 'Alice' 'was' 'unsure' 'which' 
           -> ^(CHOICE expression $trueS $falseS) |

           (conditionalStatement delimiter) => conditionalStatement delimiter
           -> conditionalStatement |

           conditionalStatement;


conditionalStatement: 'perhaps' '(' e1=expression ')' 'so' sl1=statementList elseif* ('or' sl3=statementList)? 'because' 'Alice' 'was' 'unsure' 'which'
                      -> ^(IF $e1 $sl1 elseif* $sl3?);

elseif: 'or' 'maybe' '(' e=expression ')' 'so' sl=statementList
        -> ^(IF $e $sl);


type: 'number' | 'letter' | 'sentence';
refType: 'spider' type;

expression: prec11 -> ^(EXPR prec11);
prec11: (prec10 ('||'^ prec10)*);
prec10: prec9 ('&&'^ prec9)*;
prec9: prec8 ('|'^ prec8)*;
prec8: prec7 ('^'^ prec7)*;
prec7: prec6 ('&'^ prec6)*;
prec6: prec5 (('==' | '!=')^ prec5)*;
prec5: prec4 (('>' | '<' | '>=' | '<=')^ prec4)*;
prec4: prec3 (('+' | '-')^ prec3)*;
prec3: prec2 (('*' | '/' | '%')^ prec2)*;
prec2: '!' atom -> ^('!' atom) |
       '~' atom -> ^('~' atom) |
       '+' atom -> ^('+' atom) |
       '-' atom -> ^('-' atom) |
       atom;

atom: (ID APOSTROPHE) => ID APOSTROPHE 's' expression 'piece' -> ^(ARRMEMBER ID expression) | 
      (ID callParams) => ID callParams -> ^(FUNC ID callParams) |
      ID -> ^(VAR ID) | 
      INT | 
      (APOSTROPHE) (.) (APOSTROPHE) | 
      STRING | 
      '(' prec11 ')' -> prec11;
           
delimiter: '.' | ',' | 'and' | 'but' | 'then';

/*
  Lexer rules
*/
ID: ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*;

INT: '0'..'9'+;

COMMENT: '###' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;};

WS: (' ' | '\t' | '\r' | '\n') {$channel=HIDDEN;};

STRING: '"' ( ~('"') )* '"';

APOSTROPHE: '\'';

// This prevents lexer errors, the parser should handle all errors.
CATCHALL: .;