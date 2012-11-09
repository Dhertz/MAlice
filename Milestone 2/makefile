CC = g++ -m32

compile: MAlice.o MAliceLexer.o MAliceParser.o
	$(CC) -o $@ $^ -lantlr3c

MAlice.o: MAlice.cpp
	$(CC) -c $^

MAliceLexer.o: MAliceLexer.h MAliceLexer.c
MAliceParser.o: MAliceParser.h MAliceParser.c

clean:
	rm -rf compile *.o

.phony: clean
