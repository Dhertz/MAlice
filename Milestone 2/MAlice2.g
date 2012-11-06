grammar MAlice2;

options
{
  language= Cpp;
}

program: declarationList;
declarationList : (declaration)+;

declaration: varDeclaration | funcDeclaration | procDeclaration;

varDeclaration: (ID 'was' 'a' type (('too' | 'of' expression)?) delimiter) |
                (ID 'had' expression type delimiter);

funcDeclaration: 'the' 'room' ID headerParams 'contained' 'a' type body;

procDeclaration: 'the' 'looking-glass' ID headerParams body;

headerParams: '(' (headerParamsList)? ')';
headerParamsList: (headerParam) (',' headerParam)*;

headerParam: (type | refType) ID;

callParams: '(' (callParamsList?) ')';
callParamsList: expression (',' expression)*;

// Check epsilon works
body: 'opened' ((declarationList?) statementList | ) 'closed';

statementList: (statement)+;

statement: body |
           '.' |
           expression ('became' expression | 'ate' | 'drank' | 'spoke' | 'said Alice') delimiter |
           'Alice' 'found' expression '.' |
           'what' 'was' expression '?' |
           ID callParams delimiter |
           'eventually' '(' expression ')' 'because' statementList 'enough' 'times' |
           'either' '(' expression ')' 'so' statementList 'or' statementList 'because' 'Alice' 'was' 'unsure' 'which' |
           conditionalStatement ('or' statementList)? 'because' 'Alice' 'was' 'unsure' 'which';

conditionalStatement: ('perhaps' '(' expression ')' 'so' statementList) ('or' 'maybe' '(' expression ')' 'so' statementList)*;

type: 'number' | 'letter' | 'sentence';
refType: 'spider' type;

// TODO: expression

expression: prec10 ('||' prec10)*;
prec10:	prec9 ('&&' prec9)*;
prec9: prec8 ('|' prec8)*;
prec8: prec7 ('^' prec7)*;
prec7: prec6 ('&' prec6)*;
prec6: prec5 (('=' | '!=') prec5)*;
prec5: prec4 (('>' | '<' | '>=' | '<=') prec4)*;
prec4: prec3 (('+' | '-') prec3)*;
prec3: prec2 (('*' | '/' | '%') prec2)*;
prec2: atom (('!' | '~' | '+' | '-') atom)*;

atom: ID |
      ID '\'' 's' expression 'piece' |
      STRING | CHAR | INT |
      '(' atom ')' |
      ID callParams;

ID: ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*;
STRING:	'"' .* '"';
INT: ('0'..'9')+;
CHAR: '\'' . '\'';

delimiter: '.' | ',' | 'and' | 'but' | 'then';
