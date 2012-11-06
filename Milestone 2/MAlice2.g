grammar MAlice2;

options
{
  language = Cpp;
}

program: declarationList;
declarationList : (declaration)+;

declaration: varDeclaration | funcDeclaration | procDeclaration;

varDeclaration: (ID 'was' 'a' type (('too' | 'of' expression)?) delimiter) |
                (ID 'had' expression type delimiter);

funcDeclaration: 'The' 'room' ID headerParams 'contained' 'a' type body;

procDeclaration: 'The' 'looking-glass' ID headerParams body;

headerParams: '(' (headerParamsList)? ')';
headerParamsList: (headerParam) (',' headerParam)*;

headerParam: (type | refType) ID;

callParams: '(' (callParamsList?) ')';
callParamsList: expression (',' expression)*;

// Check epsilon works
body: 'opened' ((declarationList?) statementList | ) 'closed';

statementList: (statement)+;

subRule2:  expression ('said' 'Alice' | 'spoke') delimiter; 
          

subRule: 'became' expression | 'ate' | 'drank';

//settable: (subRule | '\'' 's' expression 'piece' subRule | callParams);

statement: body |
           '.' |
           ID ('\'' 's' expression 'piece')? (('said' 'Alice' | 'spoke') | callParams | subRule) delimiter |
           (STRING | CHAR | INT) ('said' 'Alice' | 'spoke') delimiter |
           //expression ('said' 'Alice' | 'spoke') delimiter | 
           //ID settable delimiter |
           'Alice' 'found' expression '.' |
           'what' 'was' expression '?' delimiter |
           'eventually' '(' expression ')' 'because' statementList 'enough' 'times' delimiter |
           'either' '(' expression ')' 'so' statementList 'or' statementList 'because' 'Alice' 'was' 'unsure' 'which' delimiter |
           conditionalStatement ('or' statementList)? 'because' 'Alice' 'was' 'unsure' 'which' delimiter;

conditionalStatement: ('perhaps' '(' expression ')' 'so' statementList) ('or' 'maybe' '(' expression ')' 'so' statementList)*;

type: 'number' | 'letter' | 'sentence';
refType: 'spider' type;

expression: (prec10 ('||' prec10)*);
prec10:	prec9 ('&&' prec9)*;
prec9: prec8 ('|' prec8)*;
prec8: prec7 ('^' prec7)*;
prec7: prec6 ('&' prec6)*;
prec6: prec5 (('==' | '!=') prec5)*;
prec5: prec4 (('>' | '<' | '>=' | '<=') prec4)*;
prec4: prec3 (('+' | '-') prec3)*;
prec3: prec2 (('*' | '/' | '%') prec2)*;
prec2: ('!' | '~' | '+' | '-')? atom;

atom: ID ('\'' 's' expression 'piece' | callParams | ) |
      STRING | CHAR | INT | '(' expression ')';

ID: ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*;
STRING:	'"'(~'"')*'"';
INT: ('0'..'9')+;
CHAR: '\'' . '\'';

delimiter: '.' | ',' | 'and' | 'but' | 'then';
