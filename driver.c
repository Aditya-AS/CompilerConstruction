#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lexer.h"


void commentFreeCode(FILE* inputFile){

}

int main(int argc, char* argv[]){
	if(argc < 2)printf("Wrong invocation.! Try \"<exec> <filename>\"\n");
	int option;
	FILE* inputFile = fopen(argv[1],"r");
	if(inputFile == NULL)printf("%s\n","Error opening the source code. Please check file permissions etc");
	
	printf("Please enter a number between 1 - 4\n");
	printf("Press 1 for removal of comments and printing comment free code on the console\n");
	printf("Press 2 for printing token list on the console along with lexeme and line no\n");
	printf("Press 3 for parsing and verifying syntactic correctness of your code\n");
	printf("Press 4 for creating a parse tree and printing it appropriately\n");
	
	scanf("%d",&option);
	switch(option){
		case 1: commentFreeCode(inputFile);break;
		case 2: tokenList()
	}
	return 0;
}