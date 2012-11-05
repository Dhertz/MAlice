grammar MAlice2;

options
{
  language= Cpp;
}

program: declarationList;
declarationList : (declarationList?) declaration;

declaration: varDeclaration | funcDeclaration | procDeclaration;

varDelcaration: (ID 'was' 'a' type delimiter) |
                (ID 'was' 'a' type 'too' delimiter) |
                (ID 'had' expression type delimiter);

funcDeclaration: 'the' 'room' ID headerParams 'contained' 'a' type body;

procDeclaration: 'the' 'looking-glass' ID headerParams body;

headerParams: '(' (headerParamsList)? ')';
headerParamsList: ((headerParamsList ',')?) headerParam;

headerParam: (type | refType) ID;

// Check epsilon works
body: 'opened' ((declarationList?) statementList | ) 'closed';

statementList: (statementList?) statement;

statement: body |
           '.' |
           expression ('became' expression | 'ate' | 'drank' | 'spoke' | 'said Alice') delimiter |
           'Alice' 'found' expression '.' |
           'what' 'was' expression '?' |
           ID callParams delimiter |
           'eventually' '(' expression ')' 'because' statementList 'enougn' 'times' |
           'either' '(' expression ')' 'so' statementList 'or' statementList 'because' 'Alice' 'was' 'unsure' 'which' |
           conditionalStatement ('or' statementList) 'because' 'Alice' 'was' 'unsure' 'which';

conditionalStatement: ('perhaps' | conditionalStatement 'or' 'maybe') '(' expression ')' 'so' statementList;

type: 'number' | 'letter' | 'sentence';
refType: 'spider' type;

// TODO: expression

callParams: '(' (callParamsList?) ')';
callParamsList: ((callParamsList ',')?) expression;

delimiter: '.' | ',' | 'and' | 'but' | 'then';
