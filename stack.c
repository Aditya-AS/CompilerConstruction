//program to make stack
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "makeGrammar.h"
#include "stack.h"
struct stack* getStack(){
	struct stack* newStack = (struct stack*)malloc(sizeof(struct stack));
}

void push(struct stack* st,char* name){
	struct Node* element = getNode();
	element->name = name;
	element->is_token = checkToken(name);
	element->next = st->head;
	st->head = element;
	return;
} 

struct Node* pop(struct stack* st){
	struct Node* element = getNode();
	element->name = st->head->name;
	element->is_token = checkToken(name);
	element->nextNode = NULL;
	st->head = st->head->name;
	return element;
}

struct Node* top(struct stack* st){
	struct Node* newNode = getNode();
	newNode->name= st->head->name;
	newNode->nextNode = NULL;
	return newNode;
}


bool checkEmpty(struct stack * st){
	if(st->head==NULL) return true;
	else return false;
}
int main(){
	return 0;
}