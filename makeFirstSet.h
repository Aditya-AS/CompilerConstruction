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

