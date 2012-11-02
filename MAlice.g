grammar MAlice;

program:	'The' 'looking-glass' 'hatta ()' 'opened' statementList 'closed' EOF;

statementList:	statement ('and' | 'but' | 'then' | ',' | '.') statementList | printReturn;
	
printReturn:	expression 'said' 'Alice.';

statement:	declaration | assignment | step;

declaration:	(ID 'was a' ('number' | 'letter')) ('too'?);

assignment:	ID 'became' expression | ID 'became' CHAR;

step:		ID ('drank' | 'ate');

expression:	or ('^' or)*;

or:		and ('|' and)*;	

and:		add ('&' add)*;

add:		ones ('+' ones)*;

ones:		term (('/' | '*' | '%') term)*;

term:		('~' term) | ID | INT;


ID:		('a'..'z'|'A'..'Z') ('a'..'z')* (('_'('a'..'z'|'A'..'Z')) ('a'..'z')*)*;

INT:		'0'..'9'+;

CHAR:		'\'' ('a'..'z') '\'';