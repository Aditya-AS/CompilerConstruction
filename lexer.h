#include <stdio.h>
#include <stdlib.h>

int isDelim(char c);
void printBuffers();
int reloadBuffer(char** buffers, FILE* fp, buffersize buf_size);
void initializeBuffer(char** buffers,FILE* fp,buffersize buf_size);
int updateForwardPointer();
void copyString(char* dest, char* begin, char* end);
int str_to_int(char* str);
float str_to_real(char* str);
void panicRecovery();
tokenInfo* fillToken(char* def_token, char* lexeme_begin, char* forward,int type);
tokenInfo* getNextToken(FILE* fp);
FILE* getStream(FILE* fp,char** B, buffersize buf_size);