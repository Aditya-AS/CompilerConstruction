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