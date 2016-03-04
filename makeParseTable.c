// program to make parse table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

char nonTerminals[52][30] = {
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
	"temp1",
	"funCallStmt",
	"outputParameters",
	"inputParameters",
	"iterativeStmt",
	"conditionalStmt",
	"conditionalSuffix",
	"ioStmt",
	"allVar",
	"allVarNext",
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

struct treeNode* getTreeNode(char* name){
	struct treeNode* newTreeNode = (struct treeNode*)malloc(sizeof(struct treeNode));
	newTreeNode->nodeValue = name;
	newTreeNode->lexemeCurrentNode = name;
	newTreeNode->parentNodeSymbol = NULL;
	newTreeNode->NodeSymbol = name;
	newTreeNode->parent = NULL;
	newTreeNode->nextTreeNode = NULL;
	newTreeNode->isLeafNode = false;
	newTreeNode->children = NULL;
	newTreeNode->valueifNumber = -1;
	newTreeNode->lineno = -1;
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
	for(i=0;i<52;i++){
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
	for(i=0;i<52;i++){
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

struct treeNode* getNextNode(struct treeNode* tree, int lineno){
	// printf("%s\n",tree->nodeValue);
	if(tree->nextTreeNode==NULL && tree->parent!=NULL) return getNextNode(tree->parent,lineno);
	else if(tree->parent==NULL) return tree;
	tree->nextTreeNode->lineno = lineno;
	return tree->nextTreeNode;
}

struct NodeList*** makeParseTable(struct Grammar* grammar,struct firstSetList* sets,struct followSetList* followSets){

	struct NodeList*** parseTable = initializeTable(52,54);
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
 	for(i=0;i<54;i++){
 		printf("%s,",nonTerminals[i]);
 		for(j=0;j<54;j++){
 			if(parseTable[i][j]->node_list->name!=NULL) printf("%s,",parseTable[i][j]->node_list->name);
 			else printf(" ,");
 		}
 		printf("\n");
 	}
 }


struct stack* addRule(struct stack* st,struct NodeList* rule){
	struct Node* tempNode = rule->node_list;
	struct stack* st2 = getStack();
	while(tempNode!=NULL){
		push(st2,tempNode->name);
		tempNode = tempNode->next;
	}
	checkEmpty(st2);
	while(!checkEmpty(st2)){
		struct Node* tempNode = pop(st2);
		// printf("%s\n",tempNode->name);
		push(st,tempNode->name);
	}
	return st;
} 

struct treeNode* addRuleToTree(struct treeNode* parentNode,struct NodeList* rule){
	
	struct Node* tempNode = rule->node_list;
	
	struct treeNode* first = getTreeNode(tempNode->name);
	if(tempNode->is_token) first->isLeafNode = true;
	struct treeNode* temp = first;
	struct treeNode* temp2; 
	tempNode= tempNode->next;
	first->parent = parentNode;
	first->parentNodeSymbol = parentNode->NodeSymbol;

	while(tempNode!=NULL){
		temp2= getTreeNode(tempNode->name);
		temp2->parentNodeSymbol = parentNode->NodeSymbol;
		temp2->parent = parentNode;
		if(tempNode->is_token) temp2->isLeafNode = true;
		temp->nextTreeNode=temp2;
		temp=temp2;
		tempNode = tempNode->next;
	}
	parentNode->children = first;
	return first;
}

struct parseTree* parse(struct NodeList*** parseTable){
	FILE* fp = fopen("test1.txt","r");
	struct stack* st = getStack();
	int i=1;
	char* token = (char*)malloc(sizeof(char)*100);
	struct Node* temp;
	struct parseTree* tree = (struct parseTree*)malloc(sizeof(struct parseTree));
	tree->root = getTreeNode("program");
	tree->root->parentNodeSymbol="ROOT";
	tree->root->lineno=1;
	struct treeNode* tempTreeNode = tree->root;
	push(st,"TK_DOLLAR");
	push(st,"program");
	while(true){
		fscanf(fp,"%s",token);
		temp = top(st);
		// printf("token = %s at line:%d\n",token,i);
		while(strcmp(temp->name,token)!=0)
		{
			if(strcmp(temp->name,"eps")==0){
			pop(st);
			tempTreeNode = getNextNode(tempTreeNode,i);
			temp = top(st);
			// printf("SEX");
				continue;
			}
			int i = getRowIndex(temp->name);
			int j = getColIndex(token);
			pop(st);
				// printf("Current top: %s\n",temp->name);
			if(parseTable[i][j]->node_list->name!=NULL)
				{
					st = addRule(st,parseTable[i][j]);
					tempTreeNode = addRuleToTree(tempTreeNode,parseTable[i][j]);
				}
			// else{
				
			// 	while(!temp->is_token){
			// 		pop(st);
			// 		temp = top(st);
			// 	}

			}
			temp = top(st);
		}
		pop(st);
		if(strcmp(token,"TK_DOLLAR")==0 && checkEmpty(st)) {fclose(fp);return tree;}
		tempTreeNode->lineno = i;
		tempTreeNode = getNextNode(tempTreeNode,i);
		i++;
	}
}


void printTree(struct treeNode* tree){
	if(!tree->isLeafNode)printf("----,%d,----,%d,%s,%s,%s\n",tree->lineno,tree->valueifNumber,tree->parentNodeSymbol,tree->isLeafNode?"Yes":"No",tree->nodeValue);
	else printf("SomethingShitty,%d,%s,%d,%s,%s,%s\n",tree->lineno,tree->nodeValue,tree->valueifNumber,tree->parentNodeSymbol,tree->isLeafNode?"Yes":"No",tree->nodeValue);
	struct treeNode* temp = tree->children;
	while(temp!=NULL){
		// printf("%s \n",temp->nodeValue);
		printTree(temp);
		temp = temp->nextTreeNode;
	}
}

int main(int argc, char const *argv[]){
	char* filename = "grammar_list";
	struct Grammar* grammar = makeGrammar(filename);
	struct firstSetList* sets = makeFirstSetList(grammar);
	struct followSetList* followSets = makeFollowSetList(grammar,sets);
	followSets = removeDup(followSets);
	// // printSet(sets);
	struct NodeList*** parseTable = makeParseTable(grammar,sets,followSets);
	// printParseTable(parseTable);
	struct parseTree* tree= parse(parseTable);
	printTree(tree->root);
	return 0;
}

