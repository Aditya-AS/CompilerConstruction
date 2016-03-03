// program to make parse table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "makeFollowSet.c"
#include "stack.c"



char terminals[54][20] = {
	"TK_ASSIGNOP",
	"TK_COMMENT",
	"TK_FIELDID",
	"TK_ID",
	"TK_NUM",
	"TK_RNUM",
	"TK_FUNID",
	"TK_RECORDID",
	"TK_WITH",
	"TK_PARAMETERS",
	"TK_END",
	"TK_WHILE",
	"TK_INT",
	"TK_REAL",
	"TK_TYPE",
	"TK_MAIN",
	"TK_GLOBAL",
	"TK_PARAMETER",
	"TK_LIST",
	"TK_SQL",
	"TK_SQR",
	"TK_INPUT",
	"TK_OUTPUT",
	"TK_SEM",
	"TK_COLON",
	"TK_DOT",
	"TK_ENDWHILE",
	"TK_OP",
	"TK_CL",
	"TK_IF",
	"TK_THEN",
	"TK_ENDIF",
	"TK_READ",
	"TK_WRITE",
	"TK_RETURN",
	"TK_PLUS",
	"TK_MINUS",
	"TK_MUL",
	"TK_DIV",
	"TK_CALL",
	"TK_RECORD",
	"TK_ENDRECORD",
	"TK_ELSE",
	"TK_AND",
	"TK_OR",
	"TK_NOT",
	"TK_LT",
	"TK_EQ",
	"TK_GT",
	"TK_GE",
	"TK_LE",
	"TK_NE",
	"TK_COMMA",
	"TK_DOLLAR"
};

char nonTerminals[51][30] = {
	"program",
	"mainFunction",
	"otherFunctions",
	"function",
	"input_par",
	"output_par",
	"parameter_list",
	"dataType",
	"primitiveDatatype",
	"constructedDatatype",
	"remaining_list",
	"stmts",
	"typeDefinitions",
	"typeDefinition",
	"fieldDefinitions",
	"fieldDefinition",
	"moreFields",
	"declarations",
	"declaration",
	"global_or_not",
	"otherStmts",
	"stmt",
	"assignmentStmt",
	"SingleOrRecId",
	"new_24",
	"funCallStmt",
	"outputParameters",
	"inputParameters",
	"iterativeStmt",
	"conditionalStmt",
	"conditionalSuffix",
	"ioStmt",
	"allVar",
	"arithmeticExpression",
	"booleanExpression",
	"all",
	"expPrime",
	"term",
	"termPrime",
	"factor",
	"highPrecedenceOperators",
	"lowPrecedenceOperators",
	"temp",
	"var",
	"logicalOp",
	"relationalOp",
	"returnStmt",
	"optionalReturn",
	"idList",
	"more_ids"
};

struct treeNode{
	char* nodeValue;
	struct treeNode* parent;
	struct treeNode* nextTreeNode; 
}

struct parseTree{
	struct treeNode* root;
}

struct treeNode* getTreeNode(char* name){
	struct treeNode* newTreeNode = (struct treeNode*)malloc(sizeof(struct treeNode));
	newTreeNode->name = name;
	newTreeNode->parent = NULL;
	newTreeNode->nextTreeNode = NULL;
	return newTreeNode;
}

struct set* findFollowSet(struct followSetList* followSets, char* name){
	struct firstSet* temp = followSets->setList;
	while(temp!=NULL){
		if(strcmp(temp->setOf,name)==0) {
			struct set* previous = getSet();
			struct set* parent = getSet();
			struct set* tempSet2 = temp->elementList;
			struct set* tempSet  = getSet();
			while(tempSet2->nextElement!=NULL){
			// 				// previous = temp;
							tempSet = getSet();
							tempSet->element = tempSet2->element;
							if(previous->element!=NULL)previous->nextElement = tempSet;
							else parent=tempSet;
							previous=tempSet;
							tempSet2 = tempSet2->nextElement;

						}
			tempSet = getSet();
			tempSet->element = tempSet2->element;
			if(previous->element!=NULL)previous->nextElement = tempSet;
			else parent=tempSet;
			return parent;
		}
		temp = temp->nextSet;  
	}
	return getSet();
}

struct NodeList*** initializeTable(int n1 , int n2){
	struct NodeList*** RuleList;
	RuleList = (struct NodeList***)malloc(sizeof(struct NodeList)*n1);
	int i=0;
	for(i=0;i<51;i++){
		RuleList[i] = (struct NodeList**)malloc(sizeof(struct NodeList)*n2);
		int j=0;
		for(j=0;j<54;j++){
			RuleList[i][j] = (struct NodeList*)malloc(sizeof(struct NodeList));
			RuleList[i][j]->node_list=getNode();
			RuleList[i][j]->nextList=NULL;
		}
	}
	return RuleList;
}

int getRowIndex(char* name){
	int i=0;
	for(i=0;i<51;i++){
		if(strcmp(nonTerminals[i],name)==0) return i;
	}
	return -1;
}

int getColIndex(char* name){
	int i=0;
	for(i=0;i<54;i++){
		if(strcmp(terminals[i],name)==0) return i;
	}
	return -1;
}
struct NodeList*** makeParseTable(struct Grammar* grammar,struct firstSetList* sets,struct followSetList* followSets){

	struct NodeList*** parseTable = initializeTable(51,54);
	struct set* parent = getSet();
	struct set* temp = getSet();
	struct set* temp2 = getSet();
	struct set* temp3 = getSet();
	struct Rule *tempRule = grammar->rules;
	while(tempRule!=NULL){
			// printf("%s,", tempRule->start->name);
			struct NodeList* tempNodeList = tempRule->rule_list;
			while(tempNodeList!=NULL){
				struct Node* tempNode = tempNodeList->node_list;
				if(!tempNode->is_token){temp2 = findFirstSet(sets,tempNode->name);
				while(temp2!=NULL){
					if(strcmp(temp2->element,"eps")!=0){
						// printf("%s,",temp2->element);
						int i = getRowIndex(tempRule->start->name);
						int j = getColIndex(temp2->element);
						// printf("%d %d",i,j);
						if(i!=-1 && j!=-1) parseTable[i][j] = tempNodeList;
					}
					else{
						temp = findFollowSet(followSets,tempNode->name);
						while(temp!=NULL){
							int i = getRowIndex(tempRule->start->name);
							int j = getColIndex(temp->element);
							// printf("%d %d",i,j);
							if(i!=-1 && j!=-1) parseTable[i][j] = tempNodeList;
							temp=temp->nextElement;		
						}

					}
					temp2 = temp2->nextElement;
				}
			}
				else{
						if(strcmp(tempNode->name,"eps")!=0){
						// printf("%s,",tempNode->name);
						int i = getRowIndex(tempRule->start->name);
						int j = getColIndex(tempNode->name);
						// printf("%d %d",i,j);
						if(i!=-1 && j!=-1) parseTable[i][j] = tempNodeList;
					}
					
					else{
						temp = findFollowSet(followSets,tempRule->start->name);
						while(temp!=NULL){
							int i = getRowIndex(tempRule->start->name);
							int j = getColIndex(temp->element);
							// printf("%d %d",i,j);
							if(i!=-1 && j!=-1) parseTable[i][j] = tempNodeList;
							temp=temp->nextElement;		
						}

					}


			}
				tempNodeList = tempNodeList->nextList;
			// temp = temp->nextElement;
			}
			// printf("\n");
		tempRule = tempRule->nextRule;
	
	}
	return parseTable;
}

void printParseTable(struct NodeList*** parseTable){
 	int i=0,j=0;
 	printf(" ,");
 	for(j=0;j<54;j++){
 		printf("%s,",terminals[j]);
 	}
 	printf("\n");
 	for(i=0;i<51;i++){
 		printf("%s,",nonTerminals[i]);
 		for(j=0;j<54;j++){
 			if(parseTable[i][j]->node_list->name!=NULL) printf("%s,",parseTable[i][j]->node_list->name);
 			else printf(" ,");
 		}
 		printf("\n");
 	}
 }


void addRule(struct stack* st,struct NodeList* rule){
	struct Node* tempNode = tempNodeList->node_list;
	struct stack* st2 = getStack();
	while(tempNode!=NULL){
		push(st2,tempNode->name);
		tempNode = tempNode->next;
	}

	while(!checkEmpty(st2)){
		struct Node* tempNode = pop(st2);
		push(st,tempNode->name);
	}
	return;
} 

struct treeNode* addRuleToTree(struct treeNode* parentNode,struct NodeList* rule){
	
	struct Node* tempNode = tempNodeList->node_list;
	
	struct treeNode* first = getTreeNode(tempNode->name);
	struct treeNode* temp = first; 
	tempNode= tempNode->next;
	first->parent = parentNode;

	while(tempNode!=NULL){
		temp->nextTreeNode = getTreeNode(tempNode->name);
		temp->parent = parentNode;
		temp=temp->nextTreeNode;
		tempNode = tempNode->next;
	}
	return first;
}

void parse(struct NodeList*** parseTable){
	struct stack* st = getStack();
	char* token = (char*)malloc(sizeof(char)*100);
	struct Node* temp;
	struct parseTree* tree = (struct parseTree*)malloc(sizeof(struct parseTree));
	tree->root = getTreeNode("program");
	struct treeNode* tempTreeNode = tree->root;
	while(true){
		token = nextToken();
		temp = top(st);
		while(strcmp(temp->name,token)!=0){
			if(strcmp(temp->name,"eps")==0){
				pop(st);
				continue;
			}
			int i = getRowIndex(temp->name);
			int j = getColIndex(token);
			pop(st);
			if(parseTable[i][j]->node_list->name!=NULL)
				{
					addRule(st,parseTable[i][j]);
					tempTreeNode = addRuleToTree(tempTreeNode,parseTable[i][j]);
				}
			temp = top(st);
		}
		pop(st);
		if(strcmp(token,"TK_DOLLAR")==0 && checkEmpty(st)) return;
		if(tempTreeNode->nextTreeNode!=NULL) tempTreeNode = tempTreeNode->nextTreeNode;
		else {
			while(tempTreeNode!=NULL){
			if(tempTreeNode->parent!=NULL) {
				if(tempTreeNode->parent->nextTreeNode!=NULL)tempTreeNode = tempTreeNode->parent->nextTreeNode;
				else tempTreeNode = tempTreeNode->parentNode;
			else  return;
		}
	}
}


int main(int argc, char const *argv[]){
	char* filename = "grammar_list";
	struct Grammar* grammar = makeGrammar(filename);
	struct firstSetList* sets = makeFirstSetList(grammar);
	struct followSetList* followSets = makeFollowSetList(grammar,sets);
	followSets = removeDup(followSets);
	// printSet(sets);
	struct NodeList*** parseTable = makeParseTable(grammar,sets,followSets);
	printParseTable(parseTable);
	return 0;
}

