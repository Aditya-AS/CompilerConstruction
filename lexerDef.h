/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
#define ERRORS_FILE "ERRORS.txt"
#define TOKENS_FILE "TOKENS.txt"
#define MAX_LEXEME_LEN 50
#define MAX_TOKEN_LEN 30
#define KEYWORD_SIZE 30


#define NEWLINE "\n"
#define TRUE 1
#define FALSE 0

#ifndef LEXERDEF_H
#define LEXERDEF_H 
typedef char** buffer;
typedef int buffersize;

enum TYPE{INT, REAL, RECORD, OTHER};typedef enum TYPE TYPE;

struct tokenInfo{
	char* token_name;
	char* lexeme;
	int line_no;
	int token_no;	
	void* value;
	TYPE t;
	int curr_state;
};typedef struct tokenInfo tokenInfo;

#endif