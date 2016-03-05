
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
struct parseTree* parse(const char* fileName,struct NodeList*** parseTable);
