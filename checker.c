//program to check a token in the token_list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool checkToken(char *token_name)
{
	FILE *token_list;
	token_list=fopen("tokens_list","r");
	char *token = (char*)malloc(sizeof(char)*100);
	int i=0;
	bool flag=false;
	while(fscanf(token_list,"%s",token)!=EOF){
		if(strcmp(token_name,token)==0){
			fclose(token_list);
			return true;
		}
		i++;
	}
	fclose(token_list);
	return false;
}