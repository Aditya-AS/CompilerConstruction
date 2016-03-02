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
 * Stuff to do to make lexical analyzer fast and efficient.
 * 1. Input Buffering.Two buffers each of size N, where N is the size of disk block. Instead of doing read char by char, do a read and get the chars filled into the buffer. Two pointers are maintained, "lexemeBegin" and "forward". "lexemeBegin" marks the beginning of the current lexeme, whose extent we are trying to determine. "forward" scans ahead until a match is found; the exact strategy will be decided later.
 * 2. Program GetNextToken in such a way that it returns one token per call to the parser when called. This should also take care of the 
*/

 // if forward is at the end of any of the buffer, reload the other buffer.
/*
 *char getChar(char* buffers){
 * 	if(*forward == '$')return '$';
 * if(*forward == '\0')reloadBuffer(buffers,inputFile,block_size);
 *	char c = *forward;
 *	forward += 1;
 *	return c;
 * }
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "keywords.h"
#include "lexerDef.h"
/*
 * Delimiters: TK_COLON, TK_PLUS, TK_MINUS, TK_DIV, TK_MUL, TK_SQL, TK_SQR, TK_SPACE(NOT DEFINED AS OF NOW), TK_COMMA, TK_SEM, TK_NEWLINE, TK_OP, TK_CL, &&&, @@@. TK_DOT
 * 
*/

//GLOBAL VARIABLE DECLARATION
char** buffers;
node** lookup_table;
int curr_buff = 0;
int token_no = 0;
int block_size;
char* lexeme_begin;
char* forward;
char* lookahead;
FILE* inputFile;
FILE* errorsFile;
FILE* tokensFile;
FILE* keywordFile;
int first_flag = 0;
int curr_state = 0;
int line_no = 0;
int errors = 0;

typedef char** buffer;
typedef int buffersize;

struct tokenInfo{
	char token_name[30];
	char lexeme[30];
	int line_no;
	int token_no;	
	void* value;
	TYPE t;
};
typedef struct tokenInfo tokenInfo;
int isDelim(char c){
	if(c == '\n' || c == ' ' ||c == ':' ||c == ';' ||c == '+' ||c == '-' ||c == '/' ||c == '*' ||c == '[' 
		||c == ']' ||c == ',' ||c == '@' ||c == '&' ||c == '\r' || c == '(' || c==')' || c=='%' || c=='\t' || c=='.')return TRUE;
	else return FALSE;
}
void flushToken(){while(!isDelim(*forward))updateForwardPointer();}
void panicRecovery(){while(!isDelim(*forward))updateForwardPointer();}
/*
 *bool is0_9(char c){	return c>='0' && c<='9';}
 *bool isb_d(char c){	return c>='b' && c<='d';}
 *bool is2_7(char c){	return c>='2' && c<= '7';}
 *bool isa_z(char c){	return c>='a' && c<='z';}
 *bool isA_Z(char c){	return c>='A' && c<= 'Z';}
*/


/*
FILE* getStream(FILE* fp,char** B, buffersize buf_size){
	if(!first_flag){initializeBuffer(B,fp,buf_size);first_flag=1;}
	else{reloadBuffer(B,fp,buf_size);}
	return fp;
}*/
void printBuffers(){
	//forward = &buffers[curr_buff][0];
	//updateForwardPointer();updateForwardPointer();
	while(1){printf("%c",*forward);int flag = updateForwardPointer();if(!flag)break;}
}
int reloadBuffer(char** buffers, FILE* fp, buffersize buf_size){
	int read = fread(&buffers[curr_buff][0], sizeof(char), buf_size, fp);
	if(read == buf_size){
		//implies buffer is fully filled, there may be more source code to be read ahead.....
		buffers[curr_buff][read] = '\0'; //special symbol for end of buffer....
	}
	else{
		//implies buffer is only partially filled implies there is an EOF somewhere in between....
		buffers[curr_buff][read] = '$'; //implies end of source code...
	}
	//forward = &buffers[curr_buff][0]; lexeme_begin = &buffers[curr_buff][0];
	curr_buff = (curr_buff+1)%2;
	return read;
}
void initializeBuffer(char** buffers,FILE* fp,buffersize buf_size){
	curr_buff = 0;
	int read  = reloadBuffer(buffers,fp, buf_size);
	int read2 = reloadBuffer(buffers, fp, buf_size);
	forward = &buffers[curr_buff][0];
	lookahead = forward+1;
	curr_state = 0;
	return;
}
int updateForwardPointer(){
	if(forward == &buffers[curr_buff][0] + block_size){
		reloadBuffer(buffers, inputFile, block_size);
		forward = &buffers[curr_buff][0];
	}
	else if(*forward == '$'){
		return 0; 
	}
	else{
		forward = forward+1;
	}
	if(*(forward+1) != '\0'){
		lookahead = forward+1;
	}
	else{
		lookahead = &buffers[(curr_buff+1)%2][0];
	}
	// if(*lookahead == '$');
	// else if(lookahead == '\0'); lookahead = buffers[(curr_buff+1)%2][0];
	return 1;
}
tokenInfo* populateToken(char* token_name, char* lex_begin, char* lex_end, int line_no){
	tokenInfo* token_info = (tokenInfo*)malloc(sizeof(tokenInfo));
	strcpy(token_info->token_name,token_name);
	token_info->line_no  = line_no;

}
void copyString(char* dest, char* begin, char* end){
	char* src = (char*)malloc(sizeof(char)*(end-begin+2));
	char* i;
	int c = 0;
	for(i=begin;i!=end;i++){
		src[c] = *i;
		c++;
	}
	src[c++] = *end;
	src[c++] = '\0';
	strcpy(dest, src);
	free(src);
}

void getNextToken(FILE* fp){
	//FILE* f = getStream(inputFile,buffers,block_size);
	initializeBuffer(buffers,fp, block_size);
	//printf("%c",*forward);return;
	//printBuffers();
	while(TRUE){
		if(*forward == '$'){printf("%s\n","TK_DOLLAR\n");break;} 
		else if(*forward == '\0'){updateForwardPointer();} //buffer completely filled but source code not finished---> Reload buffers.
		else{
			switch(*forward)
			{
				case '%' :
					if(curr_state==0){
						printf("TK_COMMENT\n");
						//curr_state = 1;
						fflush(stdin);
						//updateForwardPointer();printf("%c",*forward);
						while(*forward != '\n'){updateForwardPointer();}//printf("%c",*forward);}

						if(*forward == '\n'){line_no++;updateForwardPointer();}
					}
					else{
						//ERROR;
						errors++;
					}
					break;
				case '[' :
					if(curr_state==0){printf("%s\n","TK_SQL");updateForwardPointer();}
					else{
						//ERROR;
						errors++;
					}
					break;
				case '_':
					if(curr_state == 0){curr_state = 36;updateForwardPointer();}//printf("%c\n",*forward);printf("%c\n",*lookahead);}
					else{
						//error;
					}
					break;
				case ']' : 
					if(curr_state==0){printf("%s\n","TK_SQR");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case ';' :
					if(curr_state==0){printf("%s\n","TK_SEM");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case ':':
					if(curr_state==0){printf("%s\n","TK_COLON");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '.':
					if(curr_state==0){printf("%s\n","TK_DOT");}
					else if(curr_state == 31){curr_state = 32;}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case ',': 
					if(curr_state == 0){printf("%s\n","TK_COMMA");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '(':
					if(curr_state==0){printf("%s\n","TK_OP");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case ')':
					if(curr_state==0){printf("%s\n","TK_CL");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '+':
					if(curr_state==0){printf("%s\n","TK_PLUS");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '*':
					if(curr_state==0){printf("%s\n","TK_MUL");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '-':
					if(curr_state==0){printf("%s\n","TK_MINUS");}
					else if(curr_state == 20)curr_state = 21;
					else if(curr_state == 21)curr_state = 22;
					else if(curr_state == 22){curr_state = 0;printf("TK_ASSIGNOP\n");}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case '/':
					if(curr_state==0){printf("%s\n","TK_DIV");updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '&':
					if(curr_state==0)curr_state=13;
					else if(curr_state == 13)curr_state++;
					else if(curr_state==14)curr_state++;
					else{
						//ERROR;
						errors++;
					}
					if(curr_state == 15){printf("%s\n","TK_AND");curr_state=0;}
					updateForwardPointer();
					break;
				case '@':
					if(curr_state==0)curr_state=16;
					else if(curr_state == 16)curr_state++;
					else if(curr_state==17)curr_state++;
					else{
						//ERROR;
						errors++;
					}
					if(curr_state == 18){printf("%s\n","TK_OR");curr_state=0;}
					updateForwardPointer();
					break;
				case '~':
					if(curr_state==0){printf("%s\n","TK_NOT");updateForwardPointer();}
					else{
						//Error;
						errors++;
					}
					break;
				case '<':
					if(curr_state == 0){curr_state = 20;if(*lookahead != '=' && *lookahead != '-'){printf("%s\n","TK_LT");curr_state=0;}}
					updateForwardPointer();
					break;
				case '>':
					if(curr_state == 0){curr_state = 27;if(*lookahead != '='){printf("%s\n","TK_GT");curr_state=0;}}
					updateForwardPointer();
				case '=':
					if(curr_state == 0)curr_state=25;
					else if(curr_state == 20){printf("%s\n","TK_LE");curr_state=0;}
					else if(curr_state== 27){printf("%s\n","TK_GE");curr_state=0;}
					else if(curr_state == 25){printf("%s\n","TK_EQ");curr_state=0;}
					else if(curr_state == 29){printf("TK_NE\n");curr_state=0;}
					updateForwardPointer();
					break;
				case '2' ... '7':
					if(curr_state == 0){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_state=0;}}

					else if(curr_state == 31){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_state=0;}}
					else if(curr_state == 32){curr_state = 33;if(isDelim(*lookahead)){printf("TK_RNUM\n");curr_state=0;}}
					else if(curr_state == 33){curr_state = 33;if(isDelim(*lookahead)){printf("TK_RNUM\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 38;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 38){curr_state = 38;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 40){curr_state = 41;if(isDelim(*lookahead)){printf("TK_ID\n");curr_state=0;}}
					else if(curr_state == 41){curr_state = 42;if(isDelim(*lookahead)){printf("TK_ID\n");curr_state=0;}}
					else if(curr_state == 42){curr_state = 42;if(isDelim(*lookahead)){printf("TK_ID\n");curr_state=0;}}
					else{
						//ERROR;
						errors++;
					}
					updateForwardPointer();
					break;
				case '0' ... '1':
					if(curr_state == 0){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_state=0;}}

					else if(curr_state == 31){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_state=0;}}
					else if(curr_state == 32){curr_state = 33;if(isDelim(*lookahead)){printf("TK_RNUM\n");curr_state=0;}}
					else if(curr_state == 33){curr_state = 33;if(isDelim(*lookahead)){printf("TK_RNUM\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 38;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 38){curr_state = 38;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					updateForwardPointer();
					break;
					//When to return this token, when you see a delimiter etc....
				case '8' ... '9':
					if(curr_state == 0){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_state=0;}}

					else if(curr_state == 31){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_state=0;}}
					else if(curr_state == 32){curr_state = 33;if(isDelim(*lookahead)){printf("TK_RNUM\n");curr_state=0;}}
					else if(curr_state == 33){curr_state = 33;if(isDelim(*lookahead)){printf("TK_RNUM\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 38;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 38){curr_state = 38;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					updateForwardPointer();
					break;
				case 'b' ... 'd':
					if(curr_state == 0)curr_state=40;
					else if(curr_state == 34){curr_state = 35;if(isDelim(*lookahead)){printf("TK_RECORDID\n");curr_state=0;}}
					else if(curr_state == 35){curr_state = 35;if(isDelim(*lookahead)){printf("TK_RECORDID\n");curr_state=0;}}
					else if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 39){curr_state = 39;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 40){curr_state = 39;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 41){curr_state = 41;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case 'a':
					if(curr_state == 0){curr_state = 39;if(isDelim(*lookahead)) {printf("TK_FIELDID\n");curr_state=0;}}
					else if(curr_state == 34){curr_state = 35;if(isDelim(*lookahead)) {printf("TK_RECORDID\n");curr_state=0;}}
					else if(curr_state == 35){curr_state = 35;if(isDelim(*lookahead)) {printf("TK_RECORDID\n");curr_state=0;}}
					else if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)) {printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)) {printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 39){curr_state = 39;if(isDelim(*lookahead)) {printf("TK_FIELDID\n");curr_state=0;}}
					else if(curr_state == 40){curr_state = 39;if(isDelim(*lookahead)) {printf("TK_FIELDID\n");curr_state=0;}}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					//printf("%c\n",*forward);
					//printf("%c\n",*lookahead);
					break;
				case 'e' ... 'z':
					//if(*forward == 'o'){printf("O Reached\n");}
					if(curr_state == 0){curr_state = 39;if(isDelim(*lookahead)) {printf("TK_FIELDID\n");curr_state=0;}}
					else if(curr_state == 39){curr_state = 39;if(isDelim(*lookahead)) {printf("TK_FIELDID\n");curr_state=0;}}
					else if(curr_state == 40){curr_state = 39;if(isDelim(*lookahead)) {printf("TK_FIELDID\n");curr_state=0;}}
					else if(curr_state == 35){curr_state = 35;if(isDelim(*lookahead)) {printf("TK_RECORDID\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)) {printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 34){curr_state = 35;if(isDelim(*lookahead)) {printf("TK_RECORDID\n");curr_state=0;}}
					else if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)) {printf("TK_FUNID\n");curr_state=0;}}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					//printf("%c \t %c\n",*forward,*lookahead);
					//printf("%c\n",*lookahead);
					break;
				case '!':
					if(curr_state == 0)curr_state = 29;
					else{
						//ERROR;
					}
					updateForwardPointer();
					break;
				case '\n':
					if(curr_state == 0)line_no++;
					updateForwardPointer();
					break;
				case '\r':
					if(curr_state==0)
					updateForwardPointer();
					break;
				case 'A' ... 'Z':
					if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)){printf("TK_FUNID\n");curr_state=0;}}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case '#':
					if(curr_state == 0){curr_state = 34;updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case ' ':
					updateForwardPointer();
					break;
				case '\t':
					updateForwardPointer();
					break;
				default:
					printf("%c\n",*forward);
					printf("%s\n","Unrecognized Token/ Lexeme\n");
					updateForwardPointer();
					break;
			}
			
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
		forward = NULL;
		lexeme_begin = NULL;
		inputFile = fopen(argv[1],"r");
		errorsFile = fopen(ERRORS_FILE,"w");
		tokensFile = fopen(TOKENS_FILE,"w");
		keywordFile = fopen(KEYWORD_FILE, "r");

		if(inputFile==NULL){fprintf(stderr, "Error! Could not open the source code file %s\n",argv[1]);exit(-1);}
		if(keywordFile==NULL){fprintf(stderr, "Error! Could not open the keywords containing file, Cannot populate lookup table without this file - %s\n",argv[1]);exit(-1);}
		if(errorsFile == NULL || tokensFile == NULL){fprintf(stderr,"Error! Could not open the errors/tokens file\n");exit(-1);}
		
		//lookup_table = createHashTable();
		//lookup_table = populateHashTable(lookup_table, keywordFile);
		// Lookup table has been initialized. Now need to start tokenizing..
		//printHashTable(lookup_table);
		getNextToken(inputFile);
		//initializeBuffer(buffers,inputFile,block_size);
		//int read = fread(buffers[0], sizeof(char), block_size, inputFile);
		//int i;
		//printBuffers();
		//initializeBuffer(buffers, inputFile, block_size);
		//getNextToken(inputFile);
	}
	return 0;
}
