struct treeNode{
	char* nodeValue;
	struct treeNode* parent;
	struct treeNode* nextTreeNode; 
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
void addRule(struct stack* st,struct NodeList* rule);
struct treeNode* addRuleToTree(struct treeNode* parentNode,struct NodeList* rule);
void parse(struct NodeList*** parseTable);
