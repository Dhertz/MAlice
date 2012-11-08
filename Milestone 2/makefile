CC = g++ -m32

compile: MAlice.o MAliceLexer.o MAliceParser.o
	$(CC) -lantlr3c -o $@ $^

MAlice.o: MAlice.cpp
	$(CC) -c $^

MAliceLexer.o: MAliceLexer.h MAliceLexer.c
MAliceParser.o: MAliceParser.h MAliceParser.c

clean:
	rm -rf hehe *.o

.phony: clean
