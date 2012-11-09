grammar MAlice;

options {
  language = C;
  ASTLabelType=pANTLR3_BASE_TREE;
  output = AST;
}

tokens {
  PROG;
  PROCDEC;
  FUNCDEC;
  HPL;
  CPL;
  VARDEC;
  BODY;
}

/*
  PARSER RULES: These rules have non-terminals on the LHS
*/
program: declarationList;
declarationList : (declaration)+;

declaration: varDeclaration | funcDeclaration | procDeclaration;

varDeclaration: ID varOptions delimiter
                -> ^(VARDEC ID varOptions);

varOptions: (('was' 'a' type ('too' | 'of' expression)?) | 'had' expression type);

funcDeclaration: 'The' 'room' ID headerParams 'contained' 'a' type body
                 -> ^(FUNCDEC 'The' 'room' ID headerParams 'contained' 'a' type body);

procDeclaration: 'The' 'looking-glass' ID headerParams body
                 -> ^(PROCDEC 'The' 'looking-glass' ID headerParams body);

headerParams: '(' (headerParamsList)? ')'
              -> ^(HPL headerParamsList?);

headerParamsList: (headerParam) (',' headerParam)*;

headerParam: (type | refType) ID;

callParams: '(' (callParamsList?) ')'
            -> ^(CPL callParamsList?);

callParamsList: expression (',' expression)*
                -> expression+;

// Check epsilon works
body: 'opened' ((declarationList?) statementList | ) 'closed'
      -> ^(BODY 'opened' statementList* 'closed');

statementList: (statement)+;

subRule2:  expression ('said' 'Alice' | 'spoke') delimiter;


subRule: 'became' expression | 'ate' | 'drank';

//settable: (subRule | '\'' 's' expression 'piece' subRule | callParams);

statement: body |
           '.' |
           ID ('\'' 's' expression 'piece')? (('said' 'Alice' | 'spoke') | callParams | subRule) delimiter |
           (STRING | CHAR | INT) ('said' 'Alice' | 'spoke') delimiter |
           // expression ('said' 'Alice' | 'spoke') delimiter |
           // ID settable delimiter |
           'Alice' 'found' expression '.' |
           'what' 'was' expression '?' |
           'eventually' '(' expression ')' 'because' statementList 'enough' 'times' |
           'either' '(' expression ')' 'so' statementList 'or' statementList 'because' 'Alice' 'was' 'unsure' 'which' (delimiter?) |
           conditionalStatement ('or' statementList)? 'because' 'Alice' 'was' 'unsure' 'which' (delimiter?);

conditionalStatement: ('perhaps' '(' expression ')' 'so' statementList) ('or' 'maybe' '(' expression ')' 'so' statementList)*;

type: 'number' | 'letter' | 'sentence';
refType: 'spider' type;

expression: (prec10 ('||'^ prec10)*);
prec10: prec9 ('&&'^ prec9)*;
prec9: prec8 ('|'^ prec8)*;
prec8: prec7 ('^'^ prec7)*;
prec7: prec6 ('&'^ prec6)*;
prec6: prec5 (('==' | '!=')^ prec5)*;
prec5: prec4 (('>' | '<' | '>=' | '<=')^ prec4)*;
prec4: prec3 (('+' | '-')^ prec3)*;
prec3: prec2 (('*' | '/' | '%')^ prec2)*;
prec2: (('!' | '~' | '+' | '-')?)^ atom;

atom: ID ('\'' 's' expression 'piece' | callParams | ) |
      STRING | '\'' CHAR '\'' | INT | '(' expression ')';

delimiter: '.' | ',' | 'and' | 'but' | 'then';

ID: ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*;
STRING: '"' (~'"')* '"';
INT: ('0'..'9')+;

// Maybe add in more characters later
CHAR: ('a'..'z'|'A'..'Z'|'_');


WS:      (' ' | '\t' | '\r' | '\n')+ {$channel = HIDDEN;};
COMMENT: '###' ~( '\r' | '\n')* {$channel = HIDDEN;};
