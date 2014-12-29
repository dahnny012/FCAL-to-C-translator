#include "scanner.h"
#include <string>
#include <cstdlib>
#include "regex.h"
#include <stdio.h>
#include <iostream>
#include <ctype.h>
using namespace std;

/*
int main()
{
	Scanner *s = new Scanner();
	string line = "Float ";
	Token *toke = s->makeToke(line);
	printf("%d %d \n",toke->terminal,floatKwd);
	cout<<"Lexeme "<< toke->lexeme << endl;
	return 1;
}*/

/*Added new constructor for the tests */
Token::Token(string input,tokenType type,Token *next_t)
{
	lexeme = input;
	terminal = type;
	next = next_t;
	lexLen = input.size();
}

Token::Token(string input,tokenType type)
{
	lexeme = input;
	terminal = type;
	next = NULL;
	lexLen = input.size();
}
Token::Token()
{
	lexeme = "";
	terminal = lexicalError;
	next = NULL;
	lexLen = 0;
}


Scanner::Scanner(Token *toke)
{
	head = toke;
	tail = toke;
	getRegx();
}

Scanner::Scanner()
{
	head = NULL;
	tail = NULL;
	getRegx();
}

void Scanner::push(Token *toke)
{
	if(head == NULL)
	{
		head = toke;
		tail = toke;
		return;
	}
	tail->next = toke;
	tail = toke;
}

Token* Scanner::pop()
{
	Token* popped;
	popped = head;
	if(head != NULL)
		head = head->next;
	return popped;
}

Token* Scanner::getHead()
{
	return head;
}


void Scanner::getRegx()
{
	
	// Initializes the Scanner::reg array
	regx[intKwd] = makeRegex("^Int"); // Int
	regx[floatKwd] = makeRegex("^Float"); // Float
	regx[stringKwd] = makeRegex("^Str"); // String
	regx[matrixKwd] = makeRegex("^Matrix"); // Matrix

	regx[boolKwd] = makeRegex("^Bool"); // bool
	regx[trueKwd] = makeRegex("^True"); // cierto
	regx[falseKwd] = makeRegex("^False"); // falso

	regx[letKwd] = makeRegex("^let"); // let
	regx[inKwd] = makeRegex("^in"); // in
	regx[endKwd] = makeRegex("^end"); // end 
	regx[ifKwd] = makeRegex("^if"); // if
	regx[thenKwd] = makeRegex("^then"); // then
	regx[elseKwd] = makeRegex("^else"); // else
	regx[forKwd] = makeRegex("^for"); // for
	regx[whileKwd] = makeRegex("^while"); // while
	regx[printKwd] = makeRegex("^print"); // print
	regx[intConst] = makeRegex("^[0-9]+"); // int constant
	regx[floatConst] = makeRegex("^([0-9]+)?\\.[0-9]+"); // float constant
	regx[stringConst] = makeRegex("^\"[^\"]*\""); // string constant
	regx[variableName] = makeRegex("^[a-zA-Z_]+[a-zA-Z0-9_]*"); // variable name
	regx[leftParen] = makeRegex("^\\("); // leftParen
	regx[rightParen] = makeRegex("^\\)"); // rightParen
	regx[leftCurly] = makeRegex("^\\{"); // leftCurly
	regx[rightCurly] = makeRegex("^\\}"); // rightCurly
	regx[leftSquare] = makeRegex("^\\["); // leftSquare
	regx[rightSquare] = makeRegex("^\\]"); // rightSquare
	regx[comma] = makeRegex("^,"); // comma
	regx[semiColon] = makeRegex("^;"); // semiColon
	regx[colon] = makeRegex("^:"); // colon
	regx[assign] = makeRegex("^="); // assign
	regx[plusSign] = makeRegex("^\\+"); // plusSign
	regx[star] = makeRegex("^\\*"); // star
	regx[dash] = makeRegex("^-"); // dash
	regx[forwardSlash] = makeRegex("^/"); // forwardSlash
	regx[lessThan] = makeRegex("^<"); // lessThan
	regx[lessThanEqual] = makeRegex("^<="); // lessThanEqual
	regx[greaterThan] = makeRegex("^>"); // greaterThan
	regx[greaterThanEqual] = makeRegex("^>="); // greaterThanEqual
	regx[equalsEquals] = makeRegex("^=="); // equalsEquals
	regx[notEquals] = makeRegex("^!="); // notEquals
	regx[andOp] = makeRegex("^&&"); // andOp
	regx[orOp] = makeRegex("^\\|\\|"); // orOp
	regx[notOp] = makeRegex("^!"); // notOp
}

// incompatiable for scan. mainly for testing regexs and proper terminal making.
Token* Scanner::makeToke(string line)
{
	
	bestMatchRegex *max = new bestMatchRegex;
	max->numMatched = 0;
	max->terminal = (tokenType)lexErr;
	const char *cLine = line.c_str();
	bestMatchRegex *temp = new bestMatchRegex;
	int i;

	for(i=0; i<MAXKWDS; i++)
	{
		//printf("i: %d bf regex \n",i);
		temp->numMatched = matchRegex(regx[i],cLine);
		temp->terminal = (tokenType)i;
			//printf("Num matched: %d \n",temp->numMatched);
		if(tokenCmp(max,temp))
		{
			tokenCpy(max,temp);
		}
	}
	
	string lex = line.substr(0,max->numMatched);
	Token *toke = new Token(lex,max->terminal);
	
	return toke;
}


// Reads a line and finds the regex with the most matched characters
// returns lexical error if no matches
Token* Scanner::makeToken(const char* line)
{
	// Initalize max,temp
	bestMatchRegex *max = new bestMatchRegex;
	max->numMatched = 0;
	max->terminal = lexicalError;
	bestMatchRegex *temp = new bestMatchRegex;
	int i;

	for(i=0; i<MAXKWDS; i++)
	{
		// Match line with reg[i] array.
		temp->numMatched = matchRegex(regx[i],line);
		temp->terminal = (tokenType)i;
		
		// if the temp's numMatch is greater than max. Temp becomes max.
		if(tokenCmp(max,temp))
		{
			tokenCpy(max,temp);
		}
	}
	
	
	//if no matches were found.
	if(max->terminal == lexicalError)
	{
		max->numMatched = 1;
	}
	
	
	string lex(line,max->numMatched);
	Token *toke = new Token(lex,max->terminal);
	//cout<<"toke "<<toke->lexeme<<endl;
	return toke;
	
}

Token* Scanner::scan(const char * code)
{
	this->head = NULL;
	this->tail = NULL;

	regex_t* whiteSpace = makeRegex("^[\n\t\r ]+") ;
    regex_t* blockComment = makeRegex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    regex_t* lineComment = makeRegex ("^//[^\n]*\n");
	 while(code[0] != '\0')
	{
		// Moves line up how many whitespace characters
		int consumed = consumeWhiteSpaceAndComments(whiteSpace,blockComment,lineComment,code);
		code = code + consumed;
		
		// if we consumed the entire char* were done.
		if(code[0] == '\0')
		{
			break;
		}
		
		// Push the token and increase the char* by the length.
		Token *temp = makeToken(code);
		push(temp);
		code = code + temp->lexLen;
	}
	
	
	 // if were here, its at the end of file.
	 push(new Token("\0",endOfFile));
	 return head;
	
}


int tokenCmp(struct bestMatchRegex *max,struct bestMatchRegex *temp)
{
	return temp->numMatched > max->numMatched;
}

void tokenCpy(struct bestMatchRegex *max,struct bestMatchRegex *temp)
{
	max->numMatched = temp->numMatched;
	max->terminal = temp->terminal;
}
int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
				  regex_t *lineComment,
                                  const char *text) {
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}

int consumeTillSpace(const char* line)
{
	int i =0;
	while(!isspace(*line))
	{
		line++;
		i++;
	}
	
	return i;
	
}




