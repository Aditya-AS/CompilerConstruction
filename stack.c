//program to make stack
#include <stdio.h>
#include <stdlib.h>
#include "makeFollowSet.c"

struct stack{
	struct Node* head;
};

struct stack* getStack(){
	struct stack* newStack = (struct stack*)malloc(sizeof(struct stack));
	newStack->head = NULL;
	return newStack;
};

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
	element->is_token = checkToken(st->head->name);
	element->next = NULL;
	st->head = st->head->next;
	return element;
}

struct Node* top(struct stack* st){
	struct Node* newNode = getNode();
	newNode->name = st->head->name;
	newNode->next = NULL;
	return newNode;
}


bool checkEmpty(struct stack * st){
	if(st->head==NULL) return true;
	else return false;
}