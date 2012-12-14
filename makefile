CPP = g++ -g -I libantlr3c
C   = gcc -I libantlr3c

all: antlr grammar codegen utils.o

antlr:
	cd libantlr3c_build/; ./configure --enable-64bit; \
	rm config.log config.status Makefile stamp-h1; rm -r .deps ; cd ../; \
	mv libantlr3c_build/antlr3config.h libantlr3c/antlr3config.h;

grammar:
	java -jar antlr-3.2.jar MAlice.g

codegen: idents/Type.o idents/Callable.o idents/Number.o idents/Sentence.o \
	     idents/Letter.o idents/Boolean.o idents/Variable.o idents/Array.o \
	     idents/Param.o idents/Function.o idents/Proc.o \
	     SymbolTable.o ast/AST.o ast/ASTNode.o ast/ExprAST.o ast/DecAST.o \
	     ast/IncAST.o ast/IfBodyAST.o ast/IfAST.o ast/ChoiceAST.o ast/WhileAST.o \
	     ast/PrintAST.o ast/ProgAST.o ast/StdinAST.o ast/VarDecAST.o \ast/VarAssignAST.o \
	     ast/ArrayAssignAST.o ast/ArrayDecAST.o ast/HeaderParamsAST.o \
	     ast/FuncDecAST.o ast/ProcDecAST.o ast/ReturnAST.o ast/CallParamsAST.o \
	     ast/FuncProcCallAST.o ast/MakeInAST.o ast/MakeOutAST.o ast/PauseAST.o \
	     ast/ReadInAST.o ast/SetHighAST.o ast/SetLowAST.o ast/PullUpAST.o \
	     TreeWalker.o Utils.o Label.o ASTVisitor.o AssemCom.o AssemFunc.o \
	     InstructionPrinter.o ExprGen.o MAlice.o FunctionOptimiser.o Optimiser.o \
	     MAliceLexer.o MAliceParser.o libantlr3c/antlr3baserecognizer.o \
	     libantlr3c/antlr3convertutf.o libantlr3c/antlr3parser.o \
	     libantlr3c/antlr3basetree.o libantlr3c/antlr3cyclicdfa.o \
	     libantlr3c/antlr3rewritestreams.o libantlr3c/antlr3basetreeadaptor.o \
	     libantlr3c/antlr3debughandlers.o libantlr3c/antlr3string.o \
	     libantlr3c/antlr3bitset.o libantlr3c/antlr3encodings.o \
	     libantlr3c/antlr3stringstream.o libantlr3c/antlr3collections.o \
	     libantlr3c/antlr3exception.o libantlr3c/antlr3tokenstream.o \
	     libantlr3c/antlr3commontoken.o libantlr3c/antlr3filestream.o \
	     libantlr3c/antlr3treeparser.o libantlr3c/antlr3commontree.o \
	     libantlr3c/antlr3inputstream.o libantlr3c/antlr3ucs2inputstream.o \
	     libantlr3c/antlr3commontreeadaptor.o libantlr3c/antlr3intstream.o \
	     libantlr3c/antlr3commontreenodestream.o libantlr3c/antlr3lexer.o
	$(CPP) -o $@ $^

.cpp.o:
	$(CPP) -c -o $*.o $<

.c.o:
	$(C) -c -o $*.o $<

utils.o:
	gcc -c utils.c

clean:
	rm -rf codegen *.o libantlr3c/*.o ast/*.o idents/*.o

.phony: all clean antlr grammar
