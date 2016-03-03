// program to make follow sets;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "makeFirstSet.c"

struct followSetList{
	struct firstSet* setList;
};

bool checkEpsilon(struct set* newSet){
	struct set* temp = newSet;
	while(temp->nextElement!=NULL){
		// printf("asd:%s\n",temp->element);
		if(strcmp(temp->element,"eps")==0) return true;
		temp = temp->nextElement;
	}
	if(strcmp(temp->element,"eps")==0) return true;
	return false;
}

bool checkNode(struct set* newSet, char* name){
	struct set* temp = newSet;
	while(temp->nextElement!=NULL){
		// printf("asd:%s\n",temp->element);
		if(temp->element!=NULL && strcmp(temp->element,name)==0) return true;
		temp = temp->nextElement;
	}
	if(temp->element!=NULL && strcmp(temp->element,name)==0) return true;
	return false;
}

bool checkRule(struct Rule* rule , char* name){
	struct NodeList* tempNodeList = rule->rule_list;
	while(tempNodeList!=NULL){
		struct Node* tempNode = tempNodeList->node_list;
		while(tempNode!=NULL){
			if(strcmp(tempNode->name,name)==0){
				return true;
			}
			tempNode = tempNode->next;
		}
		tempNodeList = tempNodeList->nextList;
	}
	return false;
}

struct set* returnNew(struct set* temp2){

			struct set* previous = getSet();
			struct set* parent = getSet();
			struct set* tempSet  = getSet();
			while(temp2->nextElement!=NULL){
			// 				// previous = temp;
							tempSet = getSet();
							tempSet->element = temp2->element;
							if(previous->element!=NULL)previous->nextElement = tempSet;
							else parent=tempSet;
							previous=tempSet;
							temp2 = temp2->nextElement;

						}
			tempSet = getSet();
			tempSet->element = temp2->element;
			if(previous->element!=NULL)previous->nextElement = tempSet;
			else parent=tempSet;
			return parent;
}

struct set* findFirstSet(struct firstSetList* firstSets, char* name){
	struct firstSet* temp = firstSets->setList;
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

struct set* checkFollowSet(struct Grammar* grammar,struct firstSetList* firstSets, char* name, char* from){
	struct Rule* tempRule = grammar->rules;
	struct set* newSet = getSet();
	struct set* parent = getSet();
	struct set* temp = getSet();
	struct set* temp2 = getSet();

	if(strcmp(name,"program")==0){
			temp = getSet();
			temp->element = "TK_DOLLAR";
			temp->nextElement = NULL;
			return temp;
		}
	// if(newSet->element==NULL)printf("%s",newSet->element);
	while(tempRule!=NULL){
		struct NodeList* tempNodeList = tempRule->rule_list;
		while(tempNodeList!=NULL){
			struct Node* tempNode = tempNodeList->node_list;
			while(tempNode!=NULL){
				if(strcmp(tempNode->name,name)==0){

					if(tempNode->next!=NULL){
						if( !tempNode->next->is_token )temp = findFirstSet(firstSets,tempNode->next->name);
						else {
							temp = getSet();
							temp->element = tempNode->next->name;
							temp->nextElement = NULL;
							// printf("%s ",temp->element);
						}
						if(newSet->element==NULL) parent = temp;
						else newSet->nextElement = temp;


						while(temp->nextElement!=NULL){
							// printf("%s ",temp->element);
							temp = temp->nextElement;
						}

						if(checkEpsilon(temp)){
							temp2 = checkFollowSet(grammar,firstSets,tempNode->next->name,";");
							temp->nextElement=temp2;
							while(temp2->nextElement!=NULL) temp2 = temp2->nextElement;
							temp=temp2;
						}

						
						// printf("%s:%s\n",tempRule->start->name,name);
						// printf("%s- ",temp->element);
						newSet = temp;
					}

					else
					{
						// printf("Entered else shit:%s \n",tempRule->start->name);
						if(strcmp(name,tempRule->start->name)!=0 && strcmp(from,tempRule->start->name)!=0)
							temp = checkFollowSet(grammar,firstSets,tempRule->start->name,name);
						temp = returnNew(temp);
						if(newSet->element==NULL) parent = temp;
						else newSet->nextElement = temp;

						while(temp->nextElement!=NULL){
							// printf("%s ",temp->element);
							temp = temp->nextElement;
						}
						newSet = temp;						 
					}
				}
				tempNode = tempNode->next;
			}
			tempNodeList = tempNodeList->nextList;
		}
		tempRule = tempRule->nextRule;
	}
	return parent;
	
}

struct followSetList* makeFollowSetList(struct Grammar* grammar , struct firstSetList* firstSets){
	struct Rule *tempRule = grammar->rules;
	struct followSetList* followSets = (struct followSetList*)malloc(sizeof(struct followSetList));
	followSets->setList = NULL;
	struct firstSet* newFirstSet = getFirstSet();
	struct firstSet* tempSet = getFirstSet();
	while(tempRule!=NULL){
		tempSet = getFirstSet();
		tempSet->setOf = tempRule->start->name;
		// printf("%s --> ",tempSet->setOf);
		
		tempSet->elementList = checkFollowSet(grammar,firstSets,tempRule->start->name,";");
		if(followSets->setList==NULL){
			newFirstSet = tempSet;
			followSets->setList = newFirstSet;
		}
		else{
			newFirstSet->nextSet = tempSet;
			 newFirstSet = tempSet ;
		}
		// printf("\n");
		tempRule = tempRule->nextRule;
	}
	
	
	return followSets;
}

void printFollowSet(struct followSetList* sets){
	struct firstSet* temp = sets->setList;
	// printf("\n\n");
	while(temp!=NULL){
		printf("%s ",temp->setOf);
		struct set* tempSet = temp->elementList;
		while(tempSet!=NULL){
			printf("%s ",tempSet->element);
			tempSet = tempSet->nextElement;
		}
		printf("\n");
		if(temp==temp->nextSet)break;
		temp = temp->nextSet;  
	}
}


struct followSetList* removeDup(struct followSetList* sets){
	struct firstSet* temp = sets->setList;
	struct followSetList* followSets = (struct followSetList*)malloc(sizeof(struct followSetList));
	struct firstSet* newFollowSet = getFirstSet();
	struct firstSet* tempFollowSet = getFirstSet();
	while(temp!=NULL){
		tempFollowSet = getFirstSet();
		tempFollowSet->setOf = temp->setOf;
		// printf("%s -> ", temp->setOf);
		struct set* tempSet = temp->elementList;
		struct set* newSet = getSet();
		struct set* parent = getSet();
		struct set* tempSet2 = getSet();
		while(tempSet!=NULL){
			if(tempSet->element!=NULL && !checkNode(parent,tempSet->element) && strcmp(tempSet->element,"eps")!=0){
				// printf("%s ",tempSet->element);
				tempSet2 = getSet();
				tempSet2->element = tempSet->element;
				tempSet2->nextElement = NULL;
				if(newSet->element==NULL) parent = tempSet2;
				else newSet->nextElement = tempSet2;
				newSet = tempSet2;
			}
			tempSet = tempSet->nextElement;
		}
		// printf("\n");
		tempFollowSet->elementList = parent;
		if(followSets->setList==NULL){
			followSets->setList = tempFollowSet;
			newFollowSet = tempFollowSet;
		}
		else{
			// printf("not null\n");
			newFollowSet->nextSet = tempFollowSet;
			newFollowSet = tempFollowSet ;
		}

		// printf("\n");
		temp = temp->nextSet;  
	}

	return followSets;
}

// int main(int argc, char const *argv[])
// {
// 	char *filename = "grammar_list";
// 	struct Grammar* grammar = makeGrammar(filename);
// 	struct firstSetList* sets = makeFirstSetList(grammar);
// 	struct followSetList* followSets = makeFollowSetList(grammar,sets);
// 	followSets = removeDup(followSets);
// 	printFollowSet(followSets);
// 	return 0;
// }
