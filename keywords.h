/*
 * Batch 13
 * Authors: Aditya Sarma, Abhinav Mehta.
 * 2013A7PS079P and 2013A7PS183P
 * f2013079@pilani.bits-pilani.ac.in & f2013183@pilani.bits-pilani.ac.in
 * Alex-1995 and Abhinav0208 - github handles.
 */
 
//Define the structure of the symbol table, the functions that will be used etc.
 
#define TABLE_SIZE 25
#define KEYWORD_FILE "keywords.txt"
struct node{
 	char* keyword;
 	char* keyword_token;
 	struct node* next;
};
typedef struct node node;


node** createHashTable();
node** populateHashTable(node** emptyHashTable, FILE* f);
void printHashTable(node** hashTable);
char* checkKeyword(node** hash_table, char* keyword);