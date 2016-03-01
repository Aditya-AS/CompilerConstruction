/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
 
 /*
  * Identifiers: [b-d][2-7][b-d]*[2-7]*
  * Function Identifiers: _[a-z|A-Z][a-z|A-Z]*[0-9]*
  * Data Types: int, real/float, record.
  * Statements: Assignment Statement, Declaration Statement, Return Statement, Iterative Statement, Conditional Statement, Function Call Statement.
  * Expressions: Arithmetic, Boolean.
*/
/*
 * A token is usually a pair consisting of a token name and an optional attribute value.
 * Each token should be of the form <TK_YYYYY, Pointer to Symbol table entry of this token>
 */
/*
 * Stuff to do to make lexical analyzer fast and efficient.
 * 1. Input Buffering.Two buffers each of size N, where N is the size of disk block. Instead of doing read char by char, do a read and get the chars filled into the buffer. Two pointers are maintained, "lexemeBegin" and "forward". "lexemeBegin" marks the beginning of the current lexeme, whose extent we are trying to determine. "forward" scans ahead until a match is found; the exact strategy will be decided later.
 * 2. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <regex.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "keywords.h"
#include "lexerDef.h"


//char* keywords = {"while","return","main","if","type","read","print","call","input","output","parameter","list","record","with"\
//			,"parameters","end","int","real","global","float","endwhile","then","endif","write","return","call","endrecord","else"};
		//Can use a hash table instead for super fast lookup but not much difference since number of keywords is any 
		
		
/*
 * Delimiters: TK_COLON, TK_PLUS, TK_MINUS, TK_DIV, TK_MUL, TK_SQL, TK_SQR, TK_SPACE(NOT DEFINED AS OF NOW), TK_COMMA, TK_SEM, TK_NEWLINE, TK_OP, TK_CL, &&&, @@@. TK_DOT
 * 
*/
//char* delimiters = {' ', ':',';','+','-','/','*','['.']',',','\n',')','(','&','@','.'};


int line_no = 0;
int errors = 0;
char** buffers;
node** lookup_table;
int curr_buff = 0;
int token_no = 0;
int block_size;
char* lexeme_begin;
char* forward;
FILE* inputFile;
FILE* errorsFile;
FILE* tokensFile;
FILE* keywordFile;
int first_flag = 0;

int reloadBuffer(char** buffers,FILE* fp, buffersize buf_size){
	int read = fread(&buffers[curr_buff][0], sizeof(char), buf_size, fp);
	if(read == buf_size){
		//implies buffer is fully filled, there may be more source code to be read ahead.....
		buffers[curr_buff][read] = '\0'; //special symbol for end of buffer....
	}
	else{
		//implies buffer is only partially filled implies there is an EOF somewhere in between....
		buffers[curr_buff][read] = '$'; //implies end of source code...
	}
	
	forward = &buffers[curr_buff][0]; lexeme_begin = &buffers[curr_buff][0];
	curr_buff = (curr_buff+1)%2;
	memset(buffers[curr_buff], '\0', buf_size); //flush the old buffer when reloading the new buffer.
	return read;
}
int initializeBuffer(char** buffers,FILE* fp,buffersize buf_size){
	curr_buff = 0;
	// f must point to a file that is already open.
	int read = reloadBuffer(buffers,fp, buf_size);
	//forward = buffers[curr_buff]; lexeme_begin = buffers[curr_buff];
	//curr_buff = (curr_buff+1)%2;
	return read;
}
FILE* getStream(FILE* fp,char** B, buffersize buf_size){
	//FILE* fp is already open, you just have to do the reading part.
	if(!first_flag){initializeBuffer(B,fp,buf_size);first_flag=1;}
	else{
		reloadBuffer(B,fp,buf_size);
	}
	return fp;
}
tokenInfo getNextToken(FILE* fp){
	getStream(inputFile,buffers,block_size);
		while(TRUE){
			while(*forward != '$'){
				if(*forward == '\0')getStream(inputFile,buffers,block_size); //buffer completely filled but source code not finished---> Reload buffers.
				else{
					
				}
			}
			if(*forward == '$'){
				printf("End of file reached\n");break;
			}
		}

}

int main(int argc, char* argv[]){
	if(argc <= 1) {fprintf(stderr,"Format: <exec> <filename.la> \n");exit(-1);}
	
	else{
		struct stat fi;
		stat("/", &fi);
		block_size = fi.st_blksize;
		
		buffers = (char**)malloc(sizeof(char*)*2); // the buffers that we will use for filling in the source code.
		buffers[0] = (char*)malloc(sizeof(char)*(block_size+1));
		buffers[1] = (char*)malloc(sizeof(char)*(block_size+1)); //one extra for the sentinel value.
	//	curr_buff = 0; // Will indicate which buffer we are currently reading 0 or 1. While updating modulo 2 should be taken care of.

		
		
		inputFile = fopen(argv[1],"r");
		errorsFile = fopen(ERRORS_FILE,"w");
		tokensFile = fopen(TOKENS_FILE,"w");
		keywordFile = fopen(KEYWORD_FILE, "r");
		if(inputFile==NULL){fprintf(stderr, "Error! Could not open the source code file %s\n",argv[1]);exit(-1);}
		if(keywordFile==NULL){fprintf(stderr, "Error! Could not open the keywords containing file, Cannot populate lookup table without this file - %s\n",argv[1]);exit(-1);}
		if(errorsFile == NULL || tokensFile == NULL){fprintf(stderr,"Error! Could not open the errors/tokens file\n");exit(-1);}
		lookup_table = createHashTable();
		lookup_table = populateHashTable(lookup_table, keywordFile);

		// Lookup table has been initialized. Now need to start tokenizing..
		printHashTable(lookup_table);
		
		//int read = initializeBuffer(buffers);
		//int read = fread(buffers[0], sizeof(char), block_size, inputFile);

		
		//	if(*forward == '\0'){reloadBuffer(buffers);}
		//	else{

		//	}
		

	}
	return 0;
}
