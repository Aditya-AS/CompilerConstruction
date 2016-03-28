/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
int isDelim(char c);
void printBuffers();
int reloadBuffer(char** buffers, FILE* fp, buffersize buf_size);
void initializeBuffer(char** buffers,FILE* fp,buffersize buf_size);
int updateForwardPointer();
void copyString(char* dest, char* begin, char* end);
int str_to_int(char* str);
float str_to_real(char* str);
tokenInfo* panicRecovery();
tokenInfo* fillToken(char* def_token, char* lexeme_begin, char* forward,int type);
tokenInfo* getNextToken(FILE* fp);
FILE* getStream(FILE* fp,char** B, buffersize buf_size);
tokenInfo* printTokenList(FILE* inputfile);