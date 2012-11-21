CPP = g++ -I . -I libantlr3c -g
C   = gcc -I libantlr3c -g

all: antlr grammar compile

antlr:
	cd libantlr3c_build/; ./configure --enable-64bit; \
	rm config.log config.status Makefile stamp-h1; rm -r .deps ; cd ../; \
	mv libantlr3c_build/antlr3config.h libantlr3c/antlr3config.h;

grammar:
	java -jar ANTLR3/antlr-3.2.jar MAlice.g

compile: ast/AST.o ast/ASTNode.o ast/ArrayAssignAST.o ast/ArrayDecAST.o ast/CallParamsAST.o ast/ChoiceAST.o ast/DecAST.o ast/FuncAST.o ast/FuncDecAST.o ast/HeaderParamsAST.o ast/IfAST.o ast/IncAST.o ast/PrintAST.o ast/ProcDecAST.o ast/ReturnAST.o ast/StdinAST.o ast/WhileAST.o idents/Type.o ast/ExprAST.o ast/VarAssignAST.o idents/Array.o idents/Function.o idents/Proc.o idents/Param.o idents/Number.o idents/Sentence.o SymbolTable.o TreeWalker.o idents/Variable.o ast/VariableDecAST.o MAlice.o MAliceLexer.o MAliceParser.o libantlr3c/antlr3baserecognizer.o libantlr3c/antlr3convertutf.o libantlr3c/antlr3parser.o libantlr3c/antlr3basetree.o libantlr3c/antlr3cyclicdfa.o libantlr3c/antlr3rewritestreams.o libantlr3c/antlr3basetreeadaptor.o libantlr3c/antlr3debughandlers.o libantlr3c/antlr3string.o libantlr3c/antlr3bitset.o libantlr3c/antlr3encodings.o libantlr3c/antlr3stringstream.o libantlr3c/antlr3collections.o libantlr3c/antlr3exception.o libantlr3c/antlr3tokenstream.o libantlr3c/antlr3commontoken.o libantlr3c/antlr3filestream.o libantlr3c/antlr3treeparser.o libantlr3c/antlr3commontree.o libantlr3c/antlr3inputstream.o libantlr3c/antlr3ucs2inputstream.o libantlr3c/antlr3commontreeadaptor.o libantlr3c/antlr3intstream.o libantlr3c/antlr3commontreenodestream.o libantlr3c/antlr3lexer.o
	$(CPP) -o $@ $^

.cpp.o:
	$(CPP) -c -o $*.o $<

.c.o:
	$(C) -c -o $*.o $<

clean:
	rm -rf compile *.o libantlr3c/*.o ast/*.o idents/*.o

.phony: all clean antlr grammar
