#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include <stdio.h>
using namespace std ;
class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.


    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */





    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("priblem: %s\n",currentToken->lexeme.c_str());
				return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
			fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan (" $&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    // A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }



    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */
    
    void test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       } ;
        int count = 39; 
        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }
    
    void tokenMaker_test(string line,tokenType terminal,string lexeme)
    {
		Token *temp;
		temp = s->makeToke(line);
		TS_ASSERT_EQUALS(temp->lexeme,lexeme);
		TS_ASSERT_EQUALS(temp->terminal,terminal);
	}
	
	// kwds
    void test_TokenMaker_intKwd ( ) {
		tokenMaker_test("Int ", intKwd, "Int" );
    }
    void test_TokenMaker_floatKwd ( ) {
		tokenMaker_test("Float ", floatKwd, "Float" );
	}
	/*Added new keywords tests Next 3*/
	void test_TokenMaker_boolKwd ( ) {
		tokenMaker_test("Bool ", boolKwd, "Bool" );
	}
	void test_TokenMaker_trueKwd ( ) {
		tokenMaker_test("True ", trueKwd, "True" );
	}
	void test_TokenMaker_falseKwd ( ) {
		tokenMaker_test("False ", falseKwd, "False" );
    }
    //old tests
    void test_TokenMaker_stringKwd ( ) {
		tokenMaker_test("Str ", stringKwd, "Str" );
    }
    void test_TokenMaker_matrixKwd ( ) {
		tokenMaker_test("Matrix ", matrixKwd, "Matrix" );
    }
    void test_TokenMaker_letKwd ( ) {
		tokenMaker_test("let: ", letKwd, "let" );
    }
    void test_TokenMaker_inKwd ( ) {
		tokenMaker_test("in: ", inKwd, "in" );
    }
    void test_TokenMaker_endKwd ( ) {
		tokenMaker_test("end ", endKwd, "end" );
    }
    void test_TokenMaker_ifKwd ( ) {
		tokenMaker_test("if( ", ifKwd, "if" );
    }
    void test_TokenMaker_thenKwd ( ) {
		tokenMaker_test("then: ", thenKwd, "then" );
    }
    void test_TokenMaker_elseKwd ( ) {
		tokenMaker_test("else( ", elseKwd, "else" );
    }
    void test_TokenMaker_forKwd ( ) {
		tokenMaker_test("for( ", forKwd, "for" );
    }
    void test_TokenMaker_whileKwd ( ) {
		tokenMaker_test("while( ", whileKwd, "while" );
    }
    void test_TokenMaker_printKwd ( ) {
		tokenMaker_test("print( ", printKwd, "print" );
    }
    
    // constants
    void test_TokenMaker_intConst ( ) {
		tokenMaker_test("12131 ", intConst, "12131" );
    }
    void test_TokenMaker_floatConst ( ) {
		tokenMaker_test("12.121 ", floatConst, "12.121" );
    }
    void test_TokenMaker_stringConst ( ) {
		tokenMaker_test("\"some string\"", stringConst, "\"some string\"" );
    }
    void test_TokenMaker_variableName ( ) {
		tokenMaker_test("foo=", variableName, "foo" );
    }
    
    //Punctuation
    void test_TokenMaker_leftParen ( ) {
		tokenMaker_test("()", leftParen, "(" );
    }
    void test_TokenMaker_rightParen ( ) {
		tokenMaker_test(") d", rightParen, ")" );
    }
    void test_TokenMaker_leftCurly ( ) {
		tokenMaker_test("{some code ", leftCurly, "{" );
    }
    void test_TokenMaker_rightCurly ( ) {
		tokenMaker_test("}some code", rightCurly, "}" );
    }
    
    void test_TokenMaker_leftSquare ( ) {
		tokenMaker_test("[12 ", leftSquare, "[" );
    }
    void test_TokenMaker_rightSquare ( ) {
		tokenMaker_test("] = ", rightSquare, "]" );
    }
    void test_TokenMaker_comma ( ) {
		tokenMaker_test(", sdd", comma, "," );
    }
    void test_TokenMaker_semiColon ( ) {
		tokenMaker_test(";\n int a =1;", semiColon, ";" );
    }
    void test_TokenMaker_colon ( ) {
		tokenMaker_test(": foo()", colon, ":" );
    }
    
    //operators
    void test_TokenMaker_assign ( ) {
		tokenMaker_test("=123 ", assign, "=" );
    }
    void test_TokenMaker_plusSign( ) {
		tokenMaker_test("+ 4", plusSign, "+" );
    }
    void test_TokenMaker_star ( ) {
		tokenMaker_test("*s", star, "*" );
    }
    void test_TokenMaker_dash ( ) {
		tokenMaker_test("- ds", dash, "-" );
    }
    void test_TokenMaker_forwardSlash ( ) {
		tokenMaker_test("/2", forwardSlash, "/" );
    }
    void test_TokenMaker_lessThan ( ) {
		tokenMaker_test("<var ", lessThan, "<" );
    }
    void test_TokenMaker_lessThanEqual ( ) {
		tokenMaker_test("<=var ", lessThanEqual, "<=" );
    }
    void test_TokenMaker_greaterThan ( ) {
		tokenMaker_test(">var ", greaterThan, ">" );
    }
    void test_TokenMaker_greaterThanEqual ( ) {
		tokenMaker_test(">= ", greaterThanEqual, ">=" );
    }
    void test_TokenMaker_equalEquals ( ) {
		tokenMaker_test("== var", equalsEquals, "==" );
    }
    void test_TokenMaker_notEquals ( ) {
		tokenMaker_test("!=var ", notEquals, "!=" );
    }
    void test_TokenMaker_andOp ( ) {
		tokenMaker_test("&& ", andOp, "&&" );
    }
    void test_TokenMaker_orOp ( ) {
		tokenMaker_test("|| ", orOp, "||" );
    }
    void test_TokenMaker_notOp ( ) {
		tokenMaker_test("!var",notOp, "!" );
    }
    
    
    
    

    

} ;
