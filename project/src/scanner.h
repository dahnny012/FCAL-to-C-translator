/* A small meaningless comment */
#ifndef SCANNER_H
#define SCANNER_H
#define MAXKWDS 43
#define lexErr -1
#include <regex.h>
#include <string>

class Token;


/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */
 
enum tokenEnumType { 

	intKwd, floatKwd,
	/*New Tokens*/ boolKwd,trueKwd,falseKwd,
	stringKwd, matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	forKwd, whileKwd, printKwd,

	// Constants
	intConst, floatConst, stringConst, 

	// Names
	variableName ,

	// Punctuation
	leftParen, rightParen, 
	leftCurly, rightCurly, 
	leftSquare, rightSquare,
	comma, semiColon, colon,

	//Operators
	assign, 
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp,

	// Special terminal types
	endOfFile ,
	lexicalError
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.






class Token {
	public:

	std::string lexeme;
	tokenType terminal;
	Token* next;
	int lexLen;

	Token(std::string input,tokenType type,Token *next_t);
	Token(std::string,tokenType);
	Token();
};

class Scanner {
	public:

	Token *head;
	Token *tail;
	regex_t* regx[MAXKWDS];
	
	// functions
	Scanner();
	Scanner(Token *toke);
	void push(Token *toke);
	Token* pop();
	Token* getHead();
	void getRegx();
	Token* makeToke(std::string);
	Token* makeToken(const char*);
	Token* scan(const char *);
};


// utility functions

typedef struct bestMatchRegex {
	int numMatched;
	tokenType terminal;
	
} bestMatchRegex;

int tokenCmp(bestMatchRegex *max,bestMatchRegex *temp);
void tokenCpy(bestMatchRegex *max,bestMatchRegex *temp);
int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment,
                                  regex_t *lineComment,
                                  const char *text) ;
int consumeTillSpace(const char* line);

#endif /* SCANNER_H */
