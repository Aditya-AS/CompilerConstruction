
struct Node{
	char* name;
	bool is_token;
	struct Node *next;
};

struct NodeList{
	struct Node *node_list;
	struct NodeList *nextList;
};

struct Rule{
	int number_of_rules;
	struct Node *start;
	struct NodeList *rule_list;
	struct Rule *nextRule;
};

struct Grammar{
	struct Rule *rules;
};

struct Rule* getRule();
struct Node* getNode();
struct NodeList* getNodeList();
struct Rule* setRule(FILE *grammmarList, char *start, struct Rule* rule);
struct Grammar* makeGrammar(char* filename);
void printGrammar(struct Grammar* grammar);
