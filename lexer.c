/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
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

 /*
 	Lexical Errors:
 		Identifier Length is greater is 20;
 		Unknown Symbol + line no
 		Unknown Pattern + line no
 		Unexpected Input %c at line no %d
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

typedef struct tokenInfo tokenInfo;
int isDelim(char c){
	if(c == '\n' || c == ' ' ||c == ':' ||c == ';' ||c == '+' ||c == '-' ||c == '/' ||c == '*' ||c == '[' 
		||c == ']' ||c == ',' ||c == '@' ||c == '&' ||c == '\r' || c == '(' || c==')' || c=='%' || c=='\t' || c=='.')return TRUE;
	else return FALSE;
}
void flushToken(){while(!isDelim(*forward))updateForwardPointer();}

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
	lexeme_begin = forward;
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

void copyString(char* dest, char* begin, char* end){
	char* i;
	int c = 0;
	for(i=begin;i!=end;i++){
		dest[c] = *i;
		c++;
	}
	dest[c++] = *end;
	dest[c++] = '\0';
}

struct tokenInfo{
	char* token_name;
	char* lexeme;
	int line_no;
	int token_no;	
	void* value;
	TYPE t;
};

int str_to_int(char* str){
	int ans = 0;int i;
	int len = strlen(str);
	for(i=len-1;i>=0;i--){
		ans = ans*10;
		ans = ans + (str[i] - '0');
		
	}
	return ans;
}

float str_to_real(char* str){
	float ans = 0;
	int lhs=0,rhs=0;
	int len = strlen(str);
	return ans;
}
void panicRecovery(){while(!isDelim(*forward))updateForwardPointer();}

tokenInfo* fillToken(char* def_token, char* lexeme_begin, char* forward,int type){
	tokenInfo* token = (tokenInfo*)malloc(sizeof(tokenInfo));
	token->token_name = (char*)malloc(sizeof(char)*MAX_TOKEN_LEN);
	token->lexeme = (char*)malloc(sizeof(char)*MAX_LEXEME_LEN);
	token->line_no = line_no;
	token->token_no = token_no;
	strcpy(token->token_name, def_token);
	copyString(token->lexeme,lexeme_begin, forward);
	//printf("%s\n",token->lexeme);

	char* c = checkKeyword(lookup_table, token->lexeme);
	if(c != NULL)strcpy(token->token_name,c);

	switch(type){
		case INT:
			token->value = (int*)malloc(sizeof(int));
			*(int*)token->value = str_to_int(token->lexeme);
			break;
		case REAL:
			token->value = (float*)malloc(sizeof(int));
			*(float*)token->value = str_to_real(token->lexeme);
			break;
		case RECORD:
			token->value = NULL;
			break;
		case OTHER:
			token->value = NULL;
			break;
		default:
			token->value = NULL;
			break;
	}
	return token;
}

tokenInfo* getNextToken(FILE* fp){
	tokenInfo* curr_token = NULL;
		
		if(*forward == '$'){curr_token = fillToken("TK_DOLLAR",lexeme_begin,forward,OTHER);} 
		else if(*forward == '\0'){updateForwardPointer();} //buffer completely filled but source code not finished---> Reload buffers.
		else{
			switch(*forward)
			{
				case '%' :
					if(curr_state==0){
						//printf("%s\n","------------------" );
						//updateForwardPointer();printf("%c",*forward);
						curr_token = fillToken("TK_COMMENT",lexeme_begin,forward,OTHER);
						//printf("%s\n","------------------" );
						while(*forward != '\n'){updateForwardPointer();}//rintf("%c",*forward);
						if(*forward == '\n'){line_no++;updateForwardPointer();}
							
					//return curr_token;
					}
					else{
						//ERROR;
						printf("Unexpected Input %c and line %d\n",*forward,line_no);
						panicRecovery(fp);
						errors++;
					}
					break;
				case '[' :
					if(curr_state==0){
						curr_token=fillToken("TK_SQL",lexeme_begin,forward,OTHER);
						updateForwardPointer();}
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
					if(curr_state==0){curr_token = fillToken("TK_SQR",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case ';' :
					if(curr_state==0){curr_token = fillToken("TK_SEM",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
						break;
				case ':':
					if(curr_state==0){curr_token = fillToken("TK_COLON",lexeme_begin,forward,OTHER);updateForwardPointer();}
				else{
						//ERROR
						errors++;
					}
					break;
				case '.':
					if(curr_state==0){curr_token = fillToken("TK_DOT",lexeme_begin,forward,OTHER);}
					else if(curr_state == 31){curr_state = 32;}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case ',': 
					if(curr_state == 0){curr_token = fillToken("TK_COMMA",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '(':
					if(curr_state==0){curr_token = fillToken("TK_OP",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case ')':
					if(curr_state==0){curr_token = fillToken("TK_CL",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '+':
					if(curr_state==0){curr_token = fillToken("TK_PLUS",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '*':
					if(curr_state==0){curr_token = fillToken("TK_MUL",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//ERROR
						errors++;
					}
					break;
				case '-':
					if(curr_state==0){curr_token = fillToken("TK_MINUS",lexeme_begin,forward,OTHER);}
					else if(curr_state == 20)curr_state = 21;
					else if(curr_state == 21)curr_state = 22;
					else if(curr_state == 22){curr_state = 0;curr_token = fillToken("TK_ASSIGNOP",lexeme_begin,forward,OTHER);}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case '/':
					if(curr_state==0){curr_token = fillToken("TK_DIV",lexeme_begin,forward,OTHER);updateForwardPointer();}
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
					if(curr_state == 15){curr_token = fillToken("TK_AND",lexeme_begin,forward,OTHER);curr_state=0;}
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
					if(curr_state == 18){curr_token = fillToken("TK_OR",lexeme_begin,forward,OTHER);curr_state=0;}
					updateForwardPointer();
					break;
				case '~':
					if(curr_state==0){curr_token = fillToken("TK_NOT",lexeme_begin,forward,OTHER);updateForwardPointer();}
					else{
						//Error;
						errors++;
					}
					break;
				case '<':
					if(curr_state == 0){curr_state = 20;if(*lookahead != '=' && *lookahead != '-'){curr_token = fillToken("TK_LT",lexeme_begin,forward,OTHER);curr_state=0;}}
					updateForwardPointer();
					break;
				case '>':
					if(curr_state == 0){curr_state = 27;if(*lookahead != '='){curr_token = fillToken("TK_GT",lexeme_begin,forward,OTHER);curr_state=0;}}
					updateForwardPointer();
				case '=':
					if(curr_state == 0)curr_state=25;
					else if(curr_state == 20){curr_token = fillToken("TK_LE",lexeme_begin,forward,OTHER);curr_state=0;}
					else if(curr_state== 27){curr_token = fillToken("TK_GE",lexeme_begin,forward,OTHER);curr_state=0;}
					else if(curr_state == 25){curr_token = fillToken("TK_EQ",lexeme_begin,forward,OTHER);curr_state=0;}
					else if(curr_state == 29){curr_token = fillToken("TK_NE",lexeme_begin,forward,OTHER);curr_state=0;}
					updateForwardPointer();
					break;
				case '2' ... '7':
					if(curr_state == 0){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 31){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 32){curr_state = 33;if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 33){curr_state = 33;if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 38;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 38){curr_state = 38;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 40){curr_state = 41;if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 41){curr_state = 42;if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 42){curr_state = 42;if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else{
						//ERROR;
						errors++;
					}
					updateForwardPointer();
					break;
				case '0' ... '1':
					if(curr_state == 0){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 31){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 32){curr_state = 33;if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 33){curr_state = 33;if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 38;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 38){curr_state = 38;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					updateForwardPointer();
					break;
					//When to return this token, when you see a delimiter etc....
				case '8' ... '9':
					if(curr_state == 0){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 31){curr_state = 31;if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 32){curr_state = 33;if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 33){curr_state = 33;if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 38;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 38){curr_state = 38;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					updateForwardPointer();
					break;
				case 'b' ... 'd':
					if(curr_state == 0)curr_state=40;
					else if(curr_state == 34){curr_state = 35;if(isDelim(*lookahead)){curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 35){curr_state = 35;if(isDelim(*lookahead)){curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 39){curr_state = 39;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 40){curr_state = 39;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 41){curr_state = 41;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else{
						//ERROR
						errors++;
					}
					updateForwardPointer();
					break;
				case 'a':
					if(curr_state == 0){curr_state = 39;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 34){curr_state = 35;if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 35){curr_state = 35;if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 39){curr_state = 39;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 40){curr_state = 39;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}}
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
					if(curr_state == 0){curr_state = 39;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 39){curr_state = 39;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 40){curr_state = 39;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 35){curr_state = 35;if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 34){curr_state = 35;if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
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
					lexeme_begin = forward;
					break;
				case '\r':
					if(curr_state==0)
					updateForwardPointer();
					lexeme_begin = forward;
					break;
				case 'A' ... 'Z':
					if(curr_state == 36){curr_state = 37;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
					else if(curr_state == 37){curr_state = 37;if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}}
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
					lexeme_begin = forward;
					break;
				case '\t':
					updateForwardPointer();
					lexeme_begin = forward;
					break;
				default:
					printf("Encountered Unknown Symbol '%c' at line %d\n",*forward,line_no);
					updateForwardPointer();
					lexeme_begin = forward;
				break;
			}
		}
		
	if(curr_token!=NULL) lexeme_begin = forward;
	return curr_token;
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
		errorsFile = fopen(ERRORS_FILE,"w+");
		tokensFile = fopen(TOKENS_FILE,"w+");
		keywordFile = fopen(KEYWORD_FILE, "r");

		if(inputFile==NULL){fprintf(stderr, "Error! Could not open the source code file %s\n",argv[1]);exit(-1);}
		if(keywordFile==NULL){fprintf(stderr, "Error! Could not open the keywords containing file, Cannot populate lookup table without this file - %s\n",argv[1]);exit(-1);}
		if(errorsFile == NULL || tokensFile == NULL){fprintf(stderr,"Error! Could not open the errors/tokens file\n");exit(-1);}

		lookup_table = createHashTable();
		lookup_table = populateHashTable(lookup_table, keywordFile);
		// Lookup table has been initialized. Now need to start tokenizing..
		//printf("lkajdlfkjad\n");
		//printHashTable(lookup_table);
		//getNextToken(inputFile);
		//initializeBuffer(buffers,inputFile,block_size);
		//int read = fread(buffers[0], sizeof(char), block_size, inputFile);
		//int i;
		//printBuffers();
		//initializeBuffer(buffers, inputFile, block_size);
		//getNextToken(inputFile);
		tokenInfo* curr_token;
		initializeBuffer(buffers,inputFile, block_size);
		//printBuffers();
		while(*forward!='$'){
			
			curr_token = getNextToken(inputFile);
			if(curr_token!=NULL)printf("%s %d %s\n",curr_token->token_name,curr_token->line_no,curr_token->lexeme);
		}
		//getNextToken(inputFile);
	}
	return 0;
}



/*
int main(){
	char* str;
	str = (char*)malloc(sizeof(char)*100);
	strcpy(str,"Aditya and Abhinav are making a Compiler as part of the Compiler Project\n");
	
	char* dest = (char*)malloc(sizeof(char)*100);;
	copyString(dest, str, str);
	printf("%s\n",dest);
// }*/

// int main(){
// 	keywordFile = fopen(KEYWORD_FILE, "r");
// 	lookup_table = createHashTable();
// 	lookup_table = populateHashTable(lookup_table, keywordFile);
// 	printHashTable(lookup_table);
// 	printf("%s\n",checkKeyword(lookup_table, "endwhile"));
// }