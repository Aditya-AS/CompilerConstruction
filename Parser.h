/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "makeFollowSet.h"

struct treeNode{
	char* nodeValue;
	char* lexemeCurrentNode;
	char* parentNodeSymbol;
	char* NodeSymbol;
	bool isLeafNode;
	int lineno;
	int valueifNumber;
	struct treeNode* parent;
	struct treeNode* nextTreeNode;
	struct treeNode* children; 
};

struct parseTree{
	struct treeNode* root;
};

struct treeNode* getTreeNode(char* name);

struct set* findFollowSet(struct followSetList* followSets, char* name);

struct NodeList*** initializeTable(int n1 , int n2);

int getRowIndex(char* name);

int getColIndex(char* name);

struct NodeList*** makeParseTable(struct Grammar* grammar,struct firstSetList* sets,struct followSetList* followSets);

void printParseTable(struct NodeList*** parseTable);

struct stack* addRule(struct stack* st,struct NodeList* rule);

struct treeNode* addRuleToTree(struct treeNode* parentNode,struct NodeList* rule);

struct parseTree* parse(FILE* inputFile,struct NodeList*** parseTable,struct firstSetList* sets);
