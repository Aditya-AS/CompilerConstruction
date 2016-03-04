#define ERRORS_FILE "ERRORS.txt"
#define TOKENS_FILE "TOKENS.txt"
#define MAX_LEXEME_LEN 50
#define MAX_TOKEN_LEN 30
#define KEYWORD_SIZE 30


#define NEWLINE "\n"
#define TRUE 1
#define FALSE 0
typedef char** buffer;
typedef int buffersize;

enum TYPE{INT, REAL, RECORD, OTHER};typedef enum TYPE TYPE;

struct tokenInfo{
	char* token_name;
	char* lexeme;
	int line_no;
	int token_no;	
	void* value;
	TYPE t;
};typedef struct tokenInfo tokenInfo;

