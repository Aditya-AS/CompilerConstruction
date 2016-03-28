all: stage1exe

stage1exe: keywords.o lexer.o checker.o makeGrammar.o makeFirstSet.o makeFollowSet.o stack.o Parser.o driver.o
	gcc -g -o stage1exe keywords.o lexer.o checker.o makeGrammar.o makeFirstSet.o makeFollowSet.o stack.o Parser.o driver.o

driver.o: keywords.o lexer.o checker.o makeGrammar.o makeFirstSet.o makeFollowSet.o stack.o Parser.o driver.c
	gcc -c driver.c

lexer: keywords.o lexerDef.h lexer.h lexer.o
	gcc -o lexer keywords.o lexer.o
 
lexer.o: keywords.o lexerDef.h lexer.h lexer.c
	gcc -c lexer.c

keywords.o: keywords.c keywords.h lexerDef.h
	gcc -c keywords.c

checker.o: checker.c checker.h
	gcc -c checker.c

makeGrammar.o: checker.o makeGrammar.c makeGrammar.h
	gcc -c makeGrammar.c

makeFirstSet.o: makeGrammar.o makeFirstSet.c makeFirstSet.h
	gcc -c makeFirstSet.c

makeFollowSet.o: makeFirstSet.o makeFollowSet.h makeFirstSet.h makeFollowSet.c
	gcc -c makeFollowSet.c

stack.o: makeGrammar.o makeGrammar.h stack.h stack.c
	gcc -c stack.c

Parser: keywords.o lexer.o lexer.h checker.o makeGrammar.o makeFirstSet.o makeFollowSet.o makeFollowSet.h stack.o stack.h Parser.h parserDef.h Parser.o
	gcc -o Parser keywords.o lexer.o checker.o makeGrammar.o makeFirstSet.o makeFollowSet.o stack.o Parser.o

Parser.o: lexer.o makeFollowSet.o stack.o Parser.h parserDef.h Parser.c
	gcc -c Parser.c

clean:
	rm -rf *.o
