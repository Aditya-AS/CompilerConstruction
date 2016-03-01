

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define KEYWORD_SIZE 30
#define TABLE_SIZE 25
#define KEYWORD_FILE "keywords.txt"

int hashkey(char* keyword){
	int len = strlen(keyword);
	int i=0;
	int hash = 0;
	while(i<len){
		hash += keyword[i];
	}
	return hash % TABLE_SIZE;
}

int main(int argc, char const *argv[])
{
	
	FILE* f = fopen(KEYWORD_FILE, "r");
	char keyword[30];
	while(fgets(keyword,30,f) != NULL){
		printf("%s",keyword);
	}
	return 0;
}