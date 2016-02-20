//program to make grammar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "checker.c"

struct Node{
	char* name;
	bool is_token;
	struct Node *next;
};

struct Rule{
	struct Node *start;
	struct Node *rule_list;
	struct Rule *nextRule;
};

struct Grammar{
	struct Rule *rules;
};

struct Rule* getRule(){
	struct Rule* newRule = (struct Rule*)malloc(sizeof(struct Rule));
	newRule->start=NULL;
	newRule->rule_list=NULL;
	newRule->nextRule=NULL;
	return newRule;
}

struct Node* getNode(){
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->name=NULL;
	newNode->is_token=false;
	newNode->next=NULL;
	return newNode;
}

struct Rule* setRule(FILE *grammmarList, char *start, struct Rule* rule){
	struct Node* startNode = getNode();
	startNode->name = start;
	startNode->is_token = checkToken(start);
	startNode->next = NULL;
	rule->start = startNode;
	struct Node* newNode;
	struct Node* temp; 
	char *name = (char*)malloc(sizeof(char)*100);
	while(fscanf(grammmarList,"%s",name)!=EOF){
		// printf("%s ",name);
		if(strcmp(name,";")==0){
			// printf("\n");
			return rule;
		}
		temp = getNode();
		temp->name = name;
		temp->is_token = checkToken(name);
		temp->next=NULL;
		if(rule->rule_list==NULL) {
			newNode = temp;
			rule->rule_list = newNode;
		}
		else{
			newNode->next=temp;
			newNode=temp;
		}
		name = (char*)malloc(sizeof(char)*100);
	}
	return rule;
}


struct Grammar* makeGrammar(char* filename){
	//printf("Reading from file: %s\n",filename);
	FILE *grammmarList;
	grammmarList=fopen(filename,"r");
	
	struct Grammar* grammar= (struct Grammar*)malloc(sizeof(struct Grammar));
	char *start = (char*)malloc(sizeof(char)*1000);
	struct Rule *newRule;
	struct Rule *temp;
	
	if(grammmarList==NULL) printf("FIle is NULL");
	while(fscanf(grammmarList,"%s",start)!=EOF){
		// printf("%s ",start);
		temp = getRule();
		temp = setRule(grammmarList,start,temp);
		
		if(grammar->rules==NULL){
			newRule = temp;
			grammar->rules = newRule;
		}
		else{
			newRule->nextRule=temp;
			newRule=newRule->nextRule;
		}
		start = (char*)malloc(sizeof(char)*1000);
		//printf("%p\n", newRule);
	}
	fclose(grammmarList);
	return grammar;
}

void printGrammar(struct Grammar* grammar){
	struct Rule *tempRule = grammar->rules;
	while(tempRule!=NULL){
		printf("%s --> ",tempRule->start->name);
		struct Node* tempNode = tempRule->rule_list;
		while(tempNode!=NULL){
			printf("%s ",tempNode->name);
			tempNode=tempNode->next;
		}
		printf("\n");
		tempRule = tempRule->nextRule;
	}
}

int main(int argc, char const *argv[])
{
	char *filename = "grammar_list";
	struct Grammar* grammar = makeGrammar(filename);
	printGrammar(grammar);
	return 0;
}