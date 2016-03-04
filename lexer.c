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

//GLOBAL VARIABLE DECLARATION
char** buffers;
node** lookup_table;
int curr_buff = 0,token_no = 0,first_flag = 0,curr_state = 0,line_no = 1,errors = 0;
int block_size;
char* lexeme_begin,*forward,*lookahead;
FILE* inputFile,*errorsFile,*tokensFile,*keywordFile;
// delimiters "'\n',' ', ':', ';', '+', '-', '/', '*','[',']',',' '@', '&', '\r','(',')','%','\t','.', '<' , '>', '='"
int isDelim(char c){
	if(c == '\n' || c == ' ' ||c == ':' ||c == ';' ||c == '+' ||c == '-' ||c == '/' ||c == '*' ||c == '[' 
		||c == ']' ||c == ',' ||c == '@' ||c == '&' ||c == '\r' || c == '(' || c==')' || c=='%' || c=='\t' || c=='.' || c == '>' || c=='<' || c=='=')return TRUE;
	else return FALSE;
}

/*
FILE* getStream(FILE* fp,char** B, buffersize buf_size){
	if(!first_flag){initializeBuffer(B,fp,buf_size);first_flag=1;}
	else{reloadBuffer(B,fp,buf_size);}
	return fp;
}*/

void printBuffers(){
	//forward = &buffers[curr_buff][0];
	//;updateForwardPointer();
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
	int read  = reloadBuffer(buffers, fp, buf_size);
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

void panicRecovery(){while(!isDelim(*forward))updateForwardPointer(); lexeme_begin = forward;curr_state=0;errors=0;}

tokenInfo* fillToken(char* def_token, char* lexeme_begin, char* forward,int type){
	int error_flag = 0;
	tokenInfo* token = (tokenInfo*)malloc(sizeof(tokenInfo));
	token->token_name = (char*)malloc(sizeof(char)*MAX_TOKEN_LEN);
	token->lexeme = (char*)malloc(sizeof(char)*MAX_LEXEME_LEN);
	token->line_no = line_no;
	token->token_no = token_no;
	while(*lexeme_begin == ' ' || *lexeme_begin == '\n' || *lexeme_begin == '\t' || *lexeme_begin == '\r')lexeme_begin++;
	while(*forward == ' ' || *forward == '\n')forward--;
	strcpy(token->token_name, def_token);
	int len = strlen(token->token_name);
	if(token->token_name[len-1] == '\n')token->token_name='\0';
	copyString(token->lexeme,lexeme_begin,forward);
	char* c = checkKeyword(lookup_table, token->lexeme);
	int lex_len = strlen(token->lexeme);
	if(!strcmp(token->token_name,"TK_ID") && lex_len > 20) printf("Token Longer than 20 chars. Error!!!\n");
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
	int comment_flag = 0;int error_flag;
	tokenInfo* curr_token = NULL;
	char* c = (char*)malloc(sizeof(char)*100);
	lexeme_begin = forward;
	while(curr_token == NULL){
			switch(*forward)
			{
				case '$':
					curr_token = fillToken("TK_DOLLAR",lexeme_begin,forward,OTHER);
				break;
				case '\0':
					curr_state = 0;
					lexeme_begin = forward;
				break;
				case '%' :
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_COMMENT",lexeme_begin,forward,OTHER);
							while(*forward != '\n'){updateForwardPointer();}
							if(*forward == '\n'){line_no++;updateForwardPointer();}
							lexeme_begin = forward;
							comment_flag = 1;
						break;
						default:
							printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							error_flag = 1;errors++;
					}
				break;
				case '[' :
					switch(curr_state){
						case 0:
							curr_token=fillToken("TK_SQL",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;error_flag = 1;
					}
				break;
				case '_':
					switch(curr_state){
						case 0:
							curr_state = 36; //lexeme_begin will not be updated now, until it reads something meaningfull and curr_token gets filled from state 36.
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;error_flag = 1;
					}
				break;
				case ']' : 
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_SQR",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case ';' :
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_SEM",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case ':':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_COLON",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							//ERROR
							errors++;
					}
				break;
				case '.':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_DOT",lexeme_begin,forward,OTHER);
						break;
						case 31:
							curr_state = 32;
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case ',': 
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_COMMA",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '(':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_OP",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);errors++;
					}
				break;
				case ')':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_CL",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '+':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_PLUS",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '*':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_MUL",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '-':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_MINUS",lexeme_begin,forward,OTHER);
						break;
						case 20:
							curr_state = 21;
						break;
						case 21:
							curr_state = 22;
						break;
					 	case 22:
					 		curr_state = 0;
					 		curr_token = fillToken("TK_ASSIGNOP",lexeme_begin,forward,OTHER);
					 	break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '/':
					switch(curr_state){
						case 0:curr_token = fillToken("TK_DIV",lexeme_begin,forward,OTHER);;break;
					default:
						//ERROR
						errors++;
					}
					break;
				case '&':
					switch(curr_state){
						case 0:
							curr_state=13;
							//printf("STATE 13\n");
						break;
						case 13:
							curr_state=14;
							//printf("STATE 14\n");
						break;
						case 14:
							curr_state=0;
							curr_token = fillToken("TK_AND",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Patternnnnnnnnnnnnnn found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '@':
					switch(curr_state){
						case 0:
							curr_state=16;
						break;
						case 16:
							curr_state=17;
						break;
						case 17:
							curr_state=0;
							curr_token = fillToken("TK_OR",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '~':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_NOT",lexeme_begin,forward,OTHER);
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '<':
					switch(curr_state){
						case 0:
							curr_state = 20;
							if(*lookahead != '=' && *lookahead != '-'){curr_token = fillToken("TK_LT",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '>':
					switch(curr_state){
						case 0:
							curr_state = 27;
							if(*lookahead != '='){curr_token = fillToken("TK_GT",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
							errors++;
					}
				break;
				case '=':
					switch(curr_state){
						case 0:
							curr_state=25;
						break;
						case 20:
							curr_token = fillToken("TK_LE",lexeme_begin,forward,OTHER);curr_state=0;
						break;
						case 27:
							curr_token = fillToken("TK_GE",lexeme_begin,forward,OTHER);curr_state=0;
						break;
						case 25:
							curr_token = fillToken("TK_EQ",lexeme_begin,forward,OTHER);curr_state=0;
						break;
						case 29:
							curr_token = fillToken("TK_NE",lexeme_begin,forward,OTHER);curr_state=0;
						break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);errors++;
					}
				break;
				case '2' ... '7':
						switch(curr_state){
							case 0:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 31:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 32:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 33:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 37:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 38:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 40:
								curr_state = 41;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 41:
								curr_state = 42;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 42:
								curr_state = 42;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);
								errors++;
						}
				break;
				case '0' ... '1':
					switch(curr_state){
							case 0:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 31:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 32:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 33:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 37:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 38:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							default: errors++;
					}
				break;
				case '8' ... '9':
					switch(curr_state){
							case  0:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 31:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){printf("TK_NUM\n");curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 32:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 33:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 37:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 38:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							default:errors++;
					}
				break;
				case 'b' ... 'd':
					switch(curr_state){
						case  0:
							curr_state=40;
						break;
						case 34:
							curr_state = 35;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 35:
							curr_state = 35;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 36:
							curr_state = 37;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 37:
							curr_state = 37;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 39:
							curr_state = 39;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 40:
							curr_state = 39;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 41:
							curr_state = 41;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						default:errors++;
					}
				break;
				case 'a':
						switch(curr_state){
							case  0:
								curr_state = 39;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 34:
								curr_state = 35;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 35:
								curr_state = 35;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 36:
								curr_state = 37;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 37:
								curr_state = 37;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 39:
								curr_state = 39;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 40:
								curr_state = 39;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							default:
								errors++;
						}
				break;
				case 'e' ... 'z':
					switch(curr_state){
							case  0:
								curr_state = 39;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 39:
								curr_state = 39;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 40:
								curr_state = 39;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 35:
								curr_state = 35;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 37:
								curr_state = 37;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 34:
								curr_state = 35;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							case 36:
								curr_state = 37;
								if(isDelim(*lookahead)) {curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							break;
							default:
								errors++;
					}
				break;
				case '!':
					switch(curr_state){
						case 0:
							curr_state = 29;
						break;
						default:
							errors++;
					}
				break;
				case '\n':
					switch(curr_state){
						case 0:
							line_no++;
							lexeme_begin = forward;
						break;
						default:
						break;
					}
					lexeme_begin = forward;
				break;
				case '\r':
					switch(curr_state){
						case 0:
							//line_no++;
							lexeme_begin = forward;
						break;
						default:
						break;
					}
				break;
				case 'A' ... 'Z':
					switch(curr_state){ 
						case 36:
							curr_state = 37;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						case 37:
							curr_state = 37;
						 	if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						default:
							errors++;
					}
				break;
				case '#':
					switch(curr_state){
						case 0:
							curr_state = 34;
						break;
						default:
							errors++;
						break;
					}
				break;
				case ' ':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);curr_state=0;
					}
				break;
				case '\v':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);curr_state=0;
					}
					break;
				case '\f':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);curr_state=0;
					}
					break;
				case '\t':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:printf("Unknown Pattern found!! at line_no %d %d\n",line_no,curr_state);curr_state=0;
					}
				break;
				default:
					curr_state = 0;
					lexeme_begin = forward;
					printf("Encountered Unknown Symbol '%c' at line %d\n",*forward,line_no);
				break;
			}
	//if(errors>0) panicRecovery();
	if(!comment_flag)updateForwardPointer();
	} 
	lexeme_begin = forward;
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
			printf("%s %s %d\n",curr_token->token_name,curr_token->lexeme,curr_token->line_no);
		}
		//getNextToken(inputFile);
	}
	return 0;
}

// int main(){
// 	keywordFile = fopen(KEYWORD_FILE, "r");
// 	lookup_table = createHashTable();
// 	lookup_table = populateHashTable(lookup_table, keywordFile);
// 	printHashTable(lookup_table);
// 	printf("%s\n",checkKeyword(lookup_table, "endwhile"));
// }