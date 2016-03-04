BUILD=keywords.o lexer.o checker.o makeGrammar.o makeFirstSet.o makeFollowSet.o stack.o makeParseTable.o
CC=gcc

all: $(BUILD) driver.c
	$(CC) -o main $(BUILD) main.c

keywords.o: keywords.h keywords.c lexerDef.h
	$(CC) -c keywords.c

lexer.o: keywords.o lexerDef.h lexer.h lexer.c
	$(CC) -o lexer keywords.o lexer.o

checker.o: checker.c checker.h
	$(CC) -c checker.c

makeGrammar.o: checker.o makeGrammar.h makeGrammar.c
	$(CC) -c checker.o makeGrammar.c

makeFirstSet.o: makeGrammar.o makeFirstSet.h makeFirstSet.c 
	$(CC) -c makeGrammar.o makeFirstSet.c

makeFollowSet.o: makeFirstSet.o makeFollowSet.h makeFollowSet.c 
	$(CC) -c makeFirstSet.o makeFollowSet.c

stack.o: makeGrammar.o stack.c
	$(CC) -c makeGrammar.o stack.c

makeParseTable.o: makeFollowSet.o stack.o lexer.o makeParseTable.h makeParseTable.c
	$(CC) -c makeFollowSet.o stack.o lexer.o makeParseTable.c

clean:
	rm -rf *.o
	rm -rf main
