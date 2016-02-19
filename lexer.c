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

char* keywords = {"while","return","main","if","type","read","print","call","input","output","parameter","list","record","with"\
			,"parameters","end","int","real","global","float","endwhile","then","endif","write","return","call","endrecord","else"};
		//Can use a hash table instead for super fast lookup but not much difference since number of keywords is any 
		
		way less.

/*
 * Delimiters: TK_COLON, TK_PLUS, TK_MINUS, TK_DIV, TK_MUL, TK_SQL, TK_SQR, TK_SPACE(NOT DEFINED AS OF NOW), TK_COMMA, TK_SEM, TK_NEWLINE, TK_OP, TK_CL, &&&, @@@. TK_DOT
 * 
*/

int main(int argc, char* argv[]){
	if(argc <= 1) {fprintf(stderr,"Format: <exec> <filename.l> ");exit(-1);}
	
	else{
		struct stat fi;
		stat("/", &fi);
		int blocksize = fi.st_blksize;
		char buffer1[blocksize];
		char buffer2[blocksize];
		int lexemeBegin, forward;
		
		//Alternatively populate the buffer1 and buffer2 to reduce overhead of one system call / char read.
		
		FILE* f = fopen(argv[1],"r");
		if(f==NULL){fprintf(stderr, "Error! Could not open the file %s",argv[1]);exit(-1);}
		FILE* fout = fopen("output_tokens.txt","w"); //output file where the list of tokens identified will be written, line by line.

		// FILE SUCCESSFULLY OPENED.
		//now read the file and make tokens accordingly.

	}
}
