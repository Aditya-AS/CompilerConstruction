/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lexer.h"
#include "lexerDef.h"
#include "Parser.h"
#include "keywords.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

extern int curr_buff;
extern int token_no;
extern int first_flag;
extern int curr_state;
extern int line_no;
extern int errors;
extern int block_size;
extern char* lexeme_begin;
extern char* forward;
extern char* lookahead;
extern FILE* inputFile,*errorsFile,*tokensFile,*keywordFile;
extern char** buffers;
extern node** lookup_table;
extern tokenInfo* token;


//-------------------------NEED TO GO INTO DRIVER
FILE* commentFreeCode(FILE* f){
	//FILE* f = fopen(inputFile ,"r");
	char* forward_copy = forward;
	while(*forward != EOF){
	
		if(*forward == '%'){while(*forward != '\n'){updateForwardPointer();}updateForwardPointer();}
		else{
			fprintf(f,"%c",*forward);
			updateForwardPointer();
		}
	
	}

	forward = forward_copy;
	fclose(f);
	return f;
	//inputFile = f;
}

tokenInfo* printTokenList(FILE* inputfile){
	tokenInfo* token = getNextToken(inputFile);
	while(strcmp(token->token_name,"TK_EOF")){
		printf("%s\t%s\t%d\n",token->token_name,token->lexeme,token->line_no);
		token = getNextToken(inputFile);
	}
	printf("%s\t%s\t%d\n",token->token_name,token->lexeme,token->line_no);
	return token;

}


int main(int argc, char* argv[]){
	if(argc < 3){printf("Wrong invocation.! Try \"<exec> <filename> <parseTreeDestFile>\"\n");return -1;}
	int option;
	tokenInfo* curr_token = NULL;
	struct stat fi;
	stat("/", &fi);
	block_size = fi.st_blksize/8;
	FILE* parseTreeOutput = fopen(argv[2],"w");
	buffers = (char**)malloc(sizeof(char*)*2); // the buffers that we will use for filling in the source code.
	buffers[0] = (char*)malloc(sizeof(char)*(block_size+1));
	buffers[1] = (char*)malloc(sizeof(char)*(block_size+1)); //one extra for the sentinel value.
	keywordFile = fopen("keywords.txt","r");
	lookup_table = createHashTable();
	populateHashTable(lookup_table,keywordFile);
	forward = NULL;
	lexeme_begin = NULL;
	FILE* keywordFile = fopen("keywords.txt","r");
	inputFile = fopen(argv[1],"r");
	FILE *inputCopyFile = inputFile;
	initializeBuffer(buffers, inputFile, block_size);
	if(inputFile == NULL){printf("%s\n","Error opening the source code. Please check file permissions etc");return -1;}
	
	printf("Please enter a number between 1 - 4\n");
	printf("Press 1 for removal of comments and printing comment free code on the console\n");
	printf("Press 2 for printing token list on the console along with lexeme and line no\n");
	printf("Press 3 for parsing and verifying syntactic correctness of your code\n");
	printf("Press 4 for creating a parse tree and printing it appropriately\n");
	
	scanf("%d",&option);
	switch(option){
		
		case 1: commentFreeCode(stdout);fclose(inputFile);break;
		case 2: printTokenList(inputFile);fclose(inputFile);break;
		case 3:	{
			// FILE* f = fopen("a.txt","w");
			// commentFreeCode(f);
			// fclose(f);
			// inputCopyFile = fopen("a.txt","r");
			// // inputFile = f;
			// inputFile = inputCopyFile;
			char* filename = "grammar_list";
			struct Grammar* grammar = makeGrammar(filename);
			struct firstSetList* sets = makeFirstSetList(grammar);
			printf("FIRST SETS AUTOMATED\n");
			struct followSetList* followSets = makeFollowSetList(grammar,sets);
			followSets = removeDup(followSets);
			printf("FOLLOW SETS AUTOMATED\n");
			struct NodeList*** parseTable = makeParseTable(grammar,sets,followSets);

			// printf("%s\n",argv[1]);
			struct parseTree* tree= parse(inputCopyFile,parseTable,sets);
			printf("SUCCESSFULLY COMPILED\n");
		}
		break;
		case 4:{
			// FILE* f = fopen("a.txt","w");
			// commentFreeCode(f);
			// //fclose(f);
			// inputCopyFile = fopen("a.txt","r");
			// inputFile = inputCopyFile;
			char* filename = "grammar_list";
			struct Grammar* grammar = makeGrammar(filename);
			struct firstSetList* sets = makeFirstSetList(grammar);
			printf("FIRST SETS AUTOMATED\n");
			struct followSetList* followSets = makeFollowSetList(grammar,sets);
			followSets = removeDup(followSets);
			printf("FOLLOW SETS AUTOMATED\n");
			struct NodeList*** parseTable = makeParseTable(grammar,sets,followSets);

			// printf("%s\n",argv[1]);
			struct parseTree* tree= parse(inputCopyFile,parseTable,sets);
			printTree(tree->root,parseTreeOutput);
			printf("SUCCESSFULLY COMPILED\n");
			fclose(parseTreeOutput);
		}
		break;
	}
	return 0;
}
