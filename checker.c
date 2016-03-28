/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 *///program to check a token in the token_list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


char token_list [55][100] = {
"TK_ASSIGNOP",
"TK_COMMENT",
"TK_FIELDID",
"TK_ID",
"TK_NUM",
"TK_RNUM",
"TK_FUNID",
"TK_RECORDID",
"TK_WITH",
"TK_PARAMETERS",
"TK_END",
"TK_WHILE",
"TK_INT",
"TK_REAL",
"TK_TYPE",
"TK_MAIN",
"TK_GLOBAL",
"TK_PARAMETER",
"TK_LIST",
"TK_SQL",
"TK_SQR",
"TK_INPUT",
"TK_OUTPUT",
"TK_SEM",
"TK_COLON",
"TK_DOT",
"TK_ENDWHILE",
"TK_OP",
"TK_CL",
"TK_IF",
"TK_THEN",
"TK_ENDIF",
"TK_READ",
"TK_WRITE",
"TK_RETURN",
"TK_PLUS",
"TK_MINUS",
"TK_MUL",
"TK_DIV",
"TK_CALL",
"TK_RECORD",
"TK_ENDRECORD",
"TK_ELSE",
"TK_AND",
"TK_OR",
"TK_NOT",
"TK_LT",
"TK_LE",
"TK_EQ",
"TK_GT",
"TK_GE",
"TK_LE",
"TK_NE",
"TK_COMMA",
"eps"
};

bool checkToken(char *token_name)
{
	// FILE *token_list = NULL;
	// token_list=fopen("tokens_list","r");
	// if(token_list==NULL)printf("add\n");
	// char *token = (char*)malloc(sizeof(char)*1000);
	int i=0;
	bool flag=false;
	while(i<55){
		if(strcmp(token_name,token_list[i])==0){
			// fclose(token_list);
			return true;
		}
		i++;
	}
	// fclose(token_list);
	return false;
}