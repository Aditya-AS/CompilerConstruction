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
tokenInfo* token;
// delimiters "'\n',' ', ':', ';', '+', '-', '/', '*','[',']',',' '@', '&', '\r','(',')','%','\t','.', '<' , '>', '='"
//EVEN UNKNOWN SYMBOLS LIKE "?" "|" "}" "{" should be included in list of delimiters
int isDelim(char c){
	int flag = FALSE;

	// 		switch(c){
	// 		case '\n':return TRUE;break;
	// 		case ' ':return TRUE;break;
	// 		case ':':return TRUE;break;
	// 		case ';':return TRUE;break;
	// 		case '+':return TRUE;break;
	// 		case '-':return TRUE;break;
	// 		case '/':return TRUE;break;
	// 		case '*':return TRUE;break;
	// 		case '[':return TRUE;break;
	// 		case ']':return TRUE;break;
	// 		case ',':return TRUE;break;
	// 		case '@':return TRUE;break;
	// 		case '&':return TRUE;break;
	// 		case '\r':return TRUE;break;
	// 		case '(':return TRUE;break;
	// 		case ')':return TRUE;break;
	// 		case '%':return TRUE;break;
	// 		case '\t':return TRUE;break;
	// 		case '.':return TRUE;break;
	// 		case '>':return TRUE;break;
	// 		case '<':return TRUE;break;
	// 		case '=':return TRUE;break;
	// 		case EOF:return TRUE;break;
	// 		case '!':return TRUE;break;

	// 		default:return FALSE;break;		
	// 	}
	// 	return flag;
	// }	
	if(c == '\n' || c == ' ' ||c == ':' ||c == ';' ||c == '+' ||c == '-' ||c == '/' ||c == '*' ||c == '[' 
		||c == ']' ||c == ',' ||c == '@' ||c == '&' ||c == '\r' || c == '(' || c==')' || c=='%' || c=='\t'
	    || c=='.' || c == '>' || c=='<' || c=='=' || c==EOF || c=='!' || c=='?' || c=='|' || c=='{' || c=='}' || c=='^' || c=='`' || c == '\v')return TRUE;
	else return FALSE;

}
/*
	FILE* getStream(FILE* fp,char** B, buffersize buf_size){
	if(!first_flag){initializeBuffer(B,fp,buf_size);first_flag=1;}
	else{reloadBuffer(B,fp,buf_size);}
	return fp;
}*/

void printBuffers(){
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
		buffers[curr_buff][read] = EOF; //implies end of source code...
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
//retunrs 0 if forward pointer reaches end of file, else returns 1
int updateForwardPointer(){
	forward = forward+1;
	if(forward == &buffers[curr_buff][0] + block_size){
		reloadBuffer(buffers, inputFile, block_size);
		forward = &buffers[curr_buff][0];
	}
	else if(*forward == EOF){
		return 0; 
	}
	

	if(*(forward+1) != '\0'){
		lookahead = forward+1;
	}
	else if(*(forward+1) == EOF)*lookahead = EOF;
	else{
		lookahead = &buffers[(curr_buff+1)%2][0];
	}
	return 1;
}

//copies all the characters between the pointers begin and end into memory location starting from where dest points to.
char* updateLexemeBegin(char* lexeme_begin){
	if(lexeme_begin == &buffers[curr_buff][0] + block_size){
		forward = &buffers[(curr_buff+1)%2][0];
	}
	else{
		lexeme_begin = lexeme_begin+1;
	}
	return lexeme_begin;
}
void copyString(char* dest, char* begin, char* end){
	char* i;
	int c = 0;
	while(begin != end){
		dest[c] = *begin;
		begin = updateLexemeBegin(begin);
		c++;
	}	
	dest[c++] = *end;
	dest[c++] = '\0';

}
//when the pointer str refers to a string representation of a integer converts the string to integer and returns it.
int str_to_int(char* str){
	int ans = 0;int i;
	int len = strlen(str);
	for(i=len-1;i>=0;i--){
		ans = ans*10;
		ans = ans + (str[i] - '0');	
	}
	return ans;
}
//when the pointer str refers to a string representation of a real number converts the string to real number and returns it.
float str_to_real(char* str){
	float ans = 0;
	int lhs=0,rhs=0;
	int len = strlen(str);
	return ans;
}
tokenInfo* fillToken(char* def_token, char* lexeme_begin, char* forward,int type){
	char* lex_begin = lexeme_begin; // local copies of lexeme_begin and forward so that the global ones don't get changed.
	char* forward_copy = forward;
	int error_flag = 0;
	token = (tokenInfo*)malloc(sizeof(tokenInfo));
	token->token_name = (char*)malloc(sizeof(char));
	token->lexeme = (char*)malloc(sizeof(char));
	token->line_no = line_no;
	token->token_no = token_no;

	if(*forward == EOF){
		strcpy(token->token_name,"TK_EOF");
		strcpy(token->lexeme,"-1");
		return token;
	}

	if(strcmp("TK_LEXERROR",def_token)){
	while(*lex_begin == ' ' || *lex_begin == '\n' || *lex_begin == '\t' || *lex_begin == '\r')lex_begin++;
	while(*forward_copy == ' ' || *forward_copy == '\n' || *forward_copy == '\t' || *forward_copy == '\r')forward_copy--;
}
	strcpy(token->token_name, def_token);

	int len = strlen(token->token_name);
	if(token->token_name[len-1] == '\n')token->token_name='\0';

	copyString(token->lexeme,lex_begin,forward_copy);

	// printf("%s\n","fillToken\n" );

	if(strcmp(token->lexeme,"_main") ==0) strcpy(token->token_name,"TK_MAIN");
	// printf("kkkkkkkkkkk------------\n");
	//printf("%s filltoken\n",token->lexeme);
	char z = '\0';
	strcat(token->lexeme,&z);
	int lex_len = strlen(token->lexeme);
//	if(token->lexeme[lex_len] == '\0')printf("asdfjsdl;fkjsd;lkj\n");
	char* c = checkKeyword(lookup_table, token->lexeme);
	//printf("yoyoyoyoyoo\n");

	if(c != NULL)strcpy(token->token_name,c);
	if(strcmp("TK_FUNID",token->token_name) && lex_len > 20 && strcmp("TK_COMMENT",token->token_name)) {strcpy(token->token_name, "TK_LEXERROR");fprintf(stderr,"Identifier Lexeme Longer than 20 chars. Error!!! at line %d and lexeme is %s\n",token->line_no,token->lexeme);return token;}
	if(strcmp("TK_FUNID",token->token_name)==0 &&  lex_len > 30 && strcmp("TK_COMMENT",token->token_name)) {strcpy(token->token_name, "TK_LEXERROR");fprintf(stderr,"Function Identifier Lexeme Longer than 30 chars. Error!!! at line %d and lexeme is %s\n",token->line_no,token->lexeme);return token;}
/*
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
	}*/
	return token;
}
//this methods is called when a transition is not possible in the DFA for the current forward pointer. This will take care of skipping characters appropriately
// and return a TK_LEXERROR token to the parser. The parser will know how to handle this.
tokenInfo* panicRecovery(char* lexeme_begin, char* forward){
	//printf("Delimiter Reached. the Delimiter is \"%c\"\n",*forward);
	char* lex = (char*)malloc(sizeof(char)*100);

	//printf("forward = %c\n",*forward);
	while(!isDelim(*forward)){
			updateForwardPointer();
		}
	//printf("Delimiter Reached. the Delimiter is \"%c\"\n",*forward);

	copyString(lex,lexeme_begin,forward);
	fprintf(stderr,"Pattern Mismatch: Unexpected lexeme \"%s\" at line_no %d\n",lex,line_no);
	lexeme_begin = forward;
	//printf("%p %p\n",lexeme_begin,forward);
	//printf("%c %c\n",*(lexeme_begin+1),*forward);
	
	//free(lex);
	tokenInfo* token;
	token = fillToken("TK_LEXERROR",lexeme_begin,forward,OTHER); //OTHER should be replaced by ERROR to signify error token. Getting compiler error as of now. i.e. ERROR is not getting updated in the enum list
	curr_state=0;
	return token;
}

tokenInfo* getNextToken(FILE* fp){
	int comment_flag = 0;int error_flag=0;
	tokenInfo* curr_token = NULL;
	// if(token == NULL){
	// 	token = (tokenInfo*)malloc(sizeof(tokenInfo));
	// 	token->token_name = (char*)malloc(sizeof(char));
	// 	token->lexeme = (char*)malloc(sizeof(char));
	// }
	// char* c = (char*)malloc(sizeof(char)*100);
	lexeme_begin = forward;
	// printf("%c\n",*forward);
	while(curr_token == NULL){
			switch(*forward)
			{
				case EOF:
					curr_token = fillToken("TK_EOF",lexeme_begin,forward,OTHER);
					return curr_token; // if EOF is reached directly return. Otherwise control flows into updateForward Pointer and Seg Fault will occur.
				break;
				case '\0':
					curr_state = 0;
					lexeme_begin = forward;
				break;
				case '%':
					switch(curr_state){
						case 0:
							
							
							curr_token = fillToken("TK_COMMENT",lexeme_begin,forward,OTHER);
							while(*forward != '\n'){updateForwardPointer();}
							if(*forward == '\n'){line_no++;updateForwardPointer();}
							lexeme_begin = forward;
							comment_flag = 1;
						break;
						default:
							error_flag = 1;
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '[' :
					switch(curr_state){
						case 0:
							curr_token=fillToken("TK_SQL",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							error_flag = 1;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '_':
					switch(curr_state){
						case 0:
							curr_state = 36; //lexeme_begin will not be updated now, until it reads something meaningful and curr_token gets filled from state 36.
						break;
						default:
							errors++;
							error_flag = 1;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case ']' : 
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_SQR",lexeme_begin,forward,OTHER);
							//printf("%s\n",curr_token->token_name);
							//printf("token filled\n");
						break;
						default:
							errors++;
							error_flag=1;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case ';' :
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_SEM",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							error_flag = 1;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case ':':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_COLON",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							error_flag = 1;
							curr_token = panicRecovery(lexeme_begin,forward);
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
						default:
							errors++;

					}
				break;
				case ',': 
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_COMMA",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '(':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_OP",lexeme_begin,forward,OTHER);
							//printf("----------------------- %c",*lexeme_begin);
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case ')':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_CL",lexeme_begin,forward,OTHER);
							//printf("%c",*forward);
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '+':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_PLUS",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '*':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_MUL",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
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
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '/':
					switch(curr_state){
						case 0:curr_token = fillToken("TK_DIV",lexeme_begin,forward,OTHER);;break;
					default:
						//ERROR
						errors++;
						curr_token = panicRecovery(lexeme_begin,forward);
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
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
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
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '~':
					switch(curr_state){
						case 0:
							curr_token = fillToken("TK_NOT",lexeme_begin,forward,OTHER);
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '<':
					switch(curr_state){
						case 0:
							curr_state = 20;
							if(*lookahead != '=' && *lookahead != '-'){curr_token = fillToken("TK_LT",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '>':
					switch(curr_state){
						case 0:
							curr_state = 27;
							if(*lookahead != '='){curr_token = fillToken("TK_GT",lexeme_begin,forward,OTHER);curr_state=0;}
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
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
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '2' ... '7':
						switch(curr_state){
							case 0:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 31:
								curr_state = 31;
								if(*lookahead != '.' && isDelim(*lookahead)){curr_token = fillToken("TK_NUM",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 32:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 33:
								curr_state = 33;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_RNUM",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 37:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 38:
								curr_state = 38;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 40:
								curr_state = 41;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 41:
								curr_state = 42;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							case 42:
								curr_state = 42;
								if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
								//printf("%c\n",*forward);
							break;
							default:
								errors++;
								//printf("Error recovery begins\n");
								//char* des = (char*)malloc(sizeof(char)*100);
								//copyString(des,lexeme_begin,forward);
								//printf("%s\n",des);
								curr_token = panicRecovery(lexeme_begin,forward);
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
							default:
								errors++;
								curr_token = panicRecovery(lexeme_begin,forward);
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
							default:
								errors++;
								curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case 'b' ... 'd':
					switch(curr_state){
						case  0:
							curr_state=40;
							//printf("%c\n",*forward);
						break;
						case 34:
							curr_state = 35;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						case 35:
							curr_state = 35;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_RECORDID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						case 36:
							curr_state = 37;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						case 37:
							curr_state = 37;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FUNID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						case 39:
							curr_state = 39;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						case 40:
							curr_state = 39;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_FIELDID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						case 41:
							curr_state = 41;
							if(isDelim(*lookahead)){curr_token = fillToken("TK_ID",lexeme_begin,forward,OTHER);curr_state=0;}
							//printf("%c\n",*forward);
						break;
						default:
							errors++;
							//printf("Error recovery begins in b...d\n");
							//char* des = (char*)malloc(sizeof(char)*100);
							//	copyString(des,lexeme_begin,forward);
							//	printf("%s\n",des);
							curr_token = panicRecovery(lexeme_begin,forward);
							//printf("Error Recovery ends\n");
							//printf("%c",*forward);
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
								curr_token = panicRecovery(lexeme_begin,forward);
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
								curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '!':
					switch(curr_state){
						case 0:
							curr_state = 29;
						break;
						default:
							errors++;
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '\n':
					switch(curr_state){
						case 0:
							line_no++;
							lexeme_begin = forward;
						break;
						default:
							curr_token = panicRecovery(lexeme_begin,forward);
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
							curr_token = panicRecovery(lexeme_begin,forward);
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
							curr_token = panicRecovery(lexeme_begin,forward);
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
							curr_token = panicRecovery(lexeme_begin,forward);
						break;
					}
				break;
				case ' ':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:
							//printf("Reached here when i wasn't supposed to\n");
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				case '\v':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:
							//curr_state=0;
						//printf("Reached here when i wasn't supposed to\n");
							curr_token = panicRecovery(lexeme_begin,forward);
							
					}
					break;
				case '\f':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:
							//curr_state=0;
							//printf("Reached here when i wasn't supposed to\n");
							curr_token = panicRecovery(lexeme_begin,forward);

					}
					break;
				case '\t':
					switch(curr_state){
						case 0:lexeme_begin = forward;break;
						default:
							//curr_state=0;
							//printf("Reached here when i wasn't supposed to\n");
							curr_token = panicRecovery(lexeme_begin,forward);
					}
				break;
				default:
					curr_state = 0;
					lexeme_begin = forward;
					printf("Encountered Unknown Symbol '%c' at line %d\n",*forward,line_no);
				break;
			}

	if(!comment_flag){updateForwardPointer();}
	} 
	lexeme_begin = forward;
	return curr_token;
}

//-------------------------NEED TO GO INTO DRIVER
/*void commentFreeCode(FILE* fp){
	FILE* f = inputFile;
	char* forward_copy = forward;
	while(*forward != EOF){
	
		if(*forward == '%'){while(*forward != '\n'){updateForwardPointer();}updateForwardPointer();}
		else{
			
			//printf("%c",*forward);
			updateForwardPointer();
		}
	
	}
	forward = forward_copy;
	inputFile = f;
}

tokenInfo* printTokenList(FILE* inputfile){
	tokenInfo* token = getNextToken(inputFile);
	while(strcmp(token->token_name,"TK_EOF")!=0){
		printf("%s\t%s\t%d\n",token->token_name,token->lexeme,token->line_no);
		token = getNextToken(inputFile);}
	return token;
	
}*/

// int main(int argc, char* argv[]){
// 	if(argc <= 1) {fprintf(stderr,"Format: <exec> <filename.la> \n");exit(-1);}
// 		struct stat fi;
// 		stat("/", &fi);
// 		block_size = fi.st_blksize;
		
// 		buffers = (char**)malloc(sizeof(char*)*2); // the buffers that we will use for filling in the source code.
// 		buffers[0] = (char*)malloc(sizeof(char)*(block_size+1));
// 		buffers[1] = (char*)malloc(sizeof(char)*(block_size+1)); //one extra for the sentinel value.
// 		forward = NULL;
// 		lexeme_begin = NULL;
// 		inputFile = fopen(argv[1],"r");

// 		keywordFile = fopen(KEYWORD_FILE, "r");
// 		lookup_table = createHashTable();
// 		lookup_table = populateHashTable(lookup_table, keywordFile);
// 		tokenInfo* curr_token;
// 		initializeBuffer(buffers,inputFile, block_size);
// 		printTokenList(inputFile);
// }
