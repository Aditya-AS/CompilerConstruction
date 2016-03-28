/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */#include <stdbool.h>
#ifndef MAKEGRAMMAR_H
#define MAKEGRAMMAR_H
struct Node{
	char* name;
	bool is_token;
	struct Node *next;
};

struct NodeList{
	struct Node *node_list;
	struct NodeList *nextList;
};

struct Rule{
	int number_of_rules;
	struct Node *start;
	struct NodeList *rule_list;
	struct Rule *nextRule;
};

struct Grammar{
	struct Rule *rules;
};

struct Rule* getRule();
struct Node* getNode();
struct NodeList* getNodeList();
struct Rule* setRule(FILE *grammmarList, char *start, struct Rule* rule);
struct Grammar* makeGrammar(char* filename);
void printGrammar(struct Grammar* grammar);
#endif