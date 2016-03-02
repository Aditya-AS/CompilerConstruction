/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */

 /* Create a hash table, this hash table will store the keywords. 
 Functions need to code for
 1. Creating an empty hash table of some fixed size k - 25 in this case.
 2. An efficient hash function.
 3. Function to insert the given keyword in the hash table, need chaining here.
 4. Function to search the hash table for presence of a certain keyword.
 5. Function to print the hash table for debugging.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "keywords.h"

char* toUpper(char* keyword){
	int len = strlen(keyword);
	int i;
	char* keyword_upper = (char*)malloc(sizeof(char)*len);
	// for(i=0;i<len;i++){
	// 	keyword_upper[i] = '\0';
	// }
	for(i=0;i<len;i++){
		char c = (char)toupper(keyword[i]);
		keyword_upper[i] = c;
	}
	return keyword_upper;
}
int hashkey(char* keyword){
	int len = strlen(keyword);
	int i=0;int power = 1;
	int hash = 0;
	while(i<len){
		hash += keyword[i]*power;
		i++;
		power = power*2;
	}
	return hash % TABLE_SIZE;
}
node** createHashTable(){
	node** hash_table;
	hash_table = (node**)malloc(sizeof(node*)*TABLE_SIZE);
	int i=0;
	for(i=0;i<TABLE_SIZE;i++)hash_table[i] = NULL;
	return hash_table;
}
node* createNode(char* keyword){
	node* n = (node*)malloc(sizeof(node));
	n->keyword = (char*)malloc(sizeof(char)*(1+strlen(keyword)));
	n->keyword_token = (char*)malloc(sizeof(char)*KEYWORD_SIZE);
	n->next = NULL;
	char* keyword_upper = toUpper(keyword);
	strcpy(n->keyword, keyword);
	
	strcpy(n->keyword_token, "TK_");
	if(keyword[0] == '_'){
		strcat(n->keyword_token, keyword+1);
	}
	else{
		char keyword_caps;
		strcat(n->keyword_token, keyword_upper);
	}
	return n;
}
void insertNode(node** emptyHashTable, node* n, int hash){
	if(emptyHashTable[hash] == NULL){
		//emptyHashTable[hash] = (node*)malloc(sizeof(node));
		emptyHashTable[hash] = n;
	}
	else{
		node* temp = emptyHashTable[hash];
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = n;
	}
	//return emptyHashTable;
}
node** populateHashTable(node** emptyHashTable, FILE* f){
	//FILE* f = fopen(KEYWORD_FILE, "r");
	char keyword[KEYWORD_SIZE];
	while(fgets(keyword, KEYWORD_SIZE, f) != NULL){
		int len = strlen(keyword);
		keyword[len-1]  = '\0';
		int hash = hashkey(keyword);
		node* n = createNode(keyword);
		insertNode(emptyHashTable, n, hash);
	}
	return emptyHashTable;
}
void printHashTable(node** hashTable){
	int i=0;
	while(i<TABLE_SIZE){
		if(hashTable[i] == NULL){
			printf("%d  -----  NULL\n",i);
		}
		else{
			printf("%d ------",i);
			node* n = hashTable[i];
			while(n->next != NULL){
				printf("%s  %s\t",n->keyword_token,n->keyword);
				n = n->next;
			}
			printf("%s  %s\n",n->keyword_token,n->keyword);
		}
		i++;
	}
}

char* checkKeyword(node** hash_table, char* keyword){
	int hash = hashkey(keyword);
	if(hash_table[hash] == NULL) return NULL;
	else{
		node* n = hash_table[hash];
		while(n != NULL){
			if(strcmp(n->keyword, keyword) == 0)return n->keyword_token;
			else{
				n = n->next;
			}
		}
		return NULL;
	}
}

/*
int main(){
	FILE* f = fopen(KEYWORD_FILE, "r");
	char keyword[30];
	node** hash_table = createHashTable();

	hash_table = populateHashTable(hash_table, f);
	
	printHashTable(hash_table);

	printf("%p\n", checkKeyword(hash_table, "endwhilea"));

	// while(fgets(keyword, 30, f) != NULL){
	// 	int len = strlen(keyword);
	// 	keyword[len-1] = '\0';
	// 	printf("%s \t %s\n",keyword, toUpper(keyword));
	// }

	//printf("%s\n",toUpper("yoyo"));
	return 0;
}*/