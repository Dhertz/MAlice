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
    FUNCDEC;
      HPL;
    VARDEC;
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
    EXPR;
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
  // TODO
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

varOptions: (('was' 'a' type ('too' | 'of' expression)?) | 'had' expression type);

funcDeclaration: 'The' 'room' ID headerParams 'contained' 'a' type body
                 -> ^(FUNCDEC ID headerParams type body);

procDeclaration: 'The' 'looking-glass' ID headerParams body
                 -> ^(PROCDEC ID headerParams body);

headerParams: '(' (headerParamsList)? ')'
              -> ^(HPL headerParamsList?);

headerParamsList: (headerParam) (',' headerParam)*;

headerParam: (type | refType) ID;

callParams: '(' (callParamsList?) ')'
            -> ^(CPL callParamsList?);

callParamsList: expression (',' expression)*
                -> expression+;

body: 'opened' ((declarationList?) statementList | ) 'closed';

statementList: (statement)+;

idOptions: APOSTROPHE 's' elem=expression 'piece' (print | 'became' val=expression | 'ate' | 'drank')? 
           -> ^(ARRMEMBER $elem print? $val? 'ate'? 'drank'?) |

           callParams print? 
           -> ^(FUNC callParams print?) |

           'became' expression
           -> ^(ASSIGN expression) |

           'ate'
           -> ^(INC) |

           'drank'
           -> ^(DEC) |

           /*nothing*/;

print: ('said' 'Alice' | 'spoke');

statement: body |
           '.' |

           (ID idOptions) => ID idOptions delimiter 
           -> ^(VARSTAT ID idOptions) |

           expression print delimiter 
           -> ^(PRINT expression) |

           'Alice' 'found' expression '.' 
           -> ^(RETURN expression) |

           'what' 'was' expression '?' (delimiter?) 
           -> ^(STDIN expression) |

           'eventually' '(' expression ')' 'because' statementList 'enough' 'times' 
           -> ^(WHILE expression statementList) |

           'either' '(' expression ')' 'so' trueS=statementList 'or' falseS=statementList 'because' 'Alice' 'was' 'unsure' 'which' (delimiter?) 
           -> ^(CHOICE expression $trueS $falseS) |

           conditionalStatement ('or' statementList)? 'because' 'Alice' 'was' 'unsure' 'which' (delimiter?)
           -> ^(IF conditionalStatement statementList);

conditionalStatement: ('perhaps' '(' e1=expression ')' 'so' sl1=statementList) ('or' 'maybe' '(' e2=expression ')' 'so' sl2=statementList)*
                      -> ^(COND $e1 $sl1 $e2* $sl2*);


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
prec2: (('!' | '~' | '+' | '-')?)^ atom;

atom: ID (APOSTROPHE 's' expression 'piece' | callParams)? | 
      INT | 
      APOSTROPHE (.) APOSTROPHE | 
      STRING | 
      '(' expression ')' -> expression;
           
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