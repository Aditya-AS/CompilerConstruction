#include <stdbool.h>
struct followSetList{
	struct firstSet* setList;
};

bool checkEpsilon(struct set* newSet);
bool checkNode(struct set* newSet, char* name);
bool checkRule(struct Rule* rule , char* name);
struct set* returnNew(struct set* temp2);
struct set* findFirstSet(struct firstSetList* firstSets, char* name);
struct set* checkFollowSet(struct Grammar* grammar,struct firstSetList* firstSets, char* name, char* from);
struct followSetList* makeFollowSetList(struct Grammar* grammar , struct firstSetList* firstSets);
void printFollowSet(struct followSetList* sets);
struct followSetList* removeDup(struct followSetList* sets);
