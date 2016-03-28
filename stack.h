/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "makeGrammar.h"

struct stack{
	struct Node* head;
};

struct stack* getStack();
void push(struct stack* st,char* name);
struct Node* pop(struct stack* st);
struct Node* top(struct stack* st);
bool checkEmpty(struct stack * st);
