grammar MAlice;

options {
  language = Cpp;
}

// Turns out we need these things to actually build C++ properly:
// The Parser requires the Lexer as an include
@parser::includes
{
   #include "MAliceLexer.hpp"
}

// This is namespace that is generated, for use in everything
@lexer::namespace { MAlice }
@parser::namespace{ MAlice }

// This is needed but I've left it as default because I don't know what it does!
@lexer::traits
{
  class MAliceLexer;
  class MAliceParser;
  typedef antlr3::Traits< MAliceLexer, MAliceParser > MAliceLexerTraits;
  typedef MAliceLexerTraits MAliceParserTraits;
}

/*
  PARSER RULES: These rules have non-terminals on the LHS
*/
program: declarationList EOF;

declarationList: (declaration)+;
declaration:     varDeclaration | funcDeclaration | procDeclaration;

procDeclaration:  'The' 'looking-glass' ID headerParams body;
headerParams:     '(' headerParamsList? ')';
headerParamsList: headerParam (',' headerParam)*;
headerParam:      'spider'? TYPE ID;

callParams: '(' (callParamsList?) ')';
callParamsList: expr (',' expr)*;

funcDeclaration: 'The' 'room' ID headerParams 'contained' 'a' TYPE body;

varDeclaration: (ID 'was' 'a' TYPE (('too' | 'of' expr)?) delimiter) |
                (ID 'had' expr TYPE delimiter);

body: 'opened' ((declarationList?) statementList | ) 'closed';

statementList: (statement)+;

idOperations: 'became' expr | 'ate' | 'drank';

statement: body |
           ID ('\'' 's' expr 'piece')? (('said' 'Alice' | 'spoke') | callParams (('said' 'Alice')?) | idOperations) delimiter |
           (STRING | CHAR | INT) ('said' 'Alice' | 'spoke') delimiter |
           'Alice' 'found' expr '.' |
           'what' 'was' expr '?' (delimiter?) |
           'eventually' '(' expr ')' 'because' statementList 'enough' 'times' (delimiter?) |
           'either' '(' expr ')' 'so' statementList 'or' statementList 'because' 'Alice' 'was' 'unsure' 'which' (delimiter?) |
           conditionalStatement ('or' statementList)? 'because' 'Alice' 'was' 'unsure' 'which' (delimiter?);/* |
           COMMENT;*/
           
conditionalStatement: ('perhaps' '(' expr ')' 'so' statementList) ('or' 'maybe' '(' expr ')' 'so' statementList)*;


expr:  prec8 ('||' prec8)*;
prec8: prec7 ('&&' prec7)*;
prec7: prec6 ('|' prec6)*;
prec6: prec5 ('^' prec5)*;
prec5: prec4 ('&' prec4)*;
prec4: prec3 (('==' | '!=') prec3)*;
prec3: prec2 (('>' | '<' | '>=' | '<=') prec2)*;
prec2: prec1 (('+' | '-') prec1)*;
prec1: prec0 (('*' | '/' | '%') prec0)*;
prec0: ('!' | '~' | '+' | '-')? atom;

atom: ID ('\'' 's' expr 'piece' | callParams)? |
      CHAR | //NEEDS WORK
      STRING |
      INT |
      '(' expr ')';


delimiter: '.' | ',' | 'and' | 'but' | 'then';

/*
  LEXER RULES: These rules don't have non-terminals on the LHS - the basic blocks
*/
TYPE:   'number' | 'letter' | 'sentence';
INT:    ('0'..'9')+;
ID:     ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*;
CHAR:   '\'' 'a'..'z' '\''; //NEEDS WORK
STRING: '"' ~('"')* '"';

WS:      (' ' | '\t' | '\r' | '\n')+ {$channel = HIDDEN;};
COMMENT: '###' ~( '\r' | '\n')* {$channel = HIDDEN;};