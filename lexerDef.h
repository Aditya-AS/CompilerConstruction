#define ERRORS_FILE "ERRORS.txt"
#define TOKENS_FILE "TOKENS.txt"
#define MAX_LEXEME_LEN 50
#define MAX_TOKEN_LEN 30

#define NEWLINE "\n"
#define TRUE 1
#define FALSE 0


enum TYPE{INT, REAL, RECORD, OTHER};typedef enum TYPE TYPE;

struct symbol_table{
	int lineno;
	int colno;
	TYPE t;
	int isGlobal;
	char identifier[30];
};

typedef struct symbol_table symbol_table;


/*struct tokenInfo{
	char* tokenname;
	symbol_table* sym_entry;
};*/


