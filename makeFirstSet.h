/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */#include "makeGrammar.h"
#ifndef MAKEFIRSTSET_H
#define MAKEFIRSTSET_H
struct set{
	char* element;
	struct set* nextElement;
};

struct firstSet{
	char* setOf;
	struct set* elementList;
	struct firstSet* nextSet;
};

struct firstSetList{
	struct firstSet* setList;
};

struct set* getSet();
struct firstSet* getFirstSet();
struct set* checkFirstSet(struct Grammar* grammar, char* name);
struct firstSetList* makeFirstSetList(struct Grammar* grammar);
void printSet(struct firstSetList* sets);

#endif