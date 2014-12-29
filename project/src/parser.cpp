/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge        

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 13, 2013.

*/
#include "parser.h"
//#include "scanner.h"
#include "extToken.h"
#include <stdio.h>
#include "AST.h"
#include <assert.h>
using namespace std ;

Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}



Parser::Parser ( ) { 
    currToken = NULL; prevToken = NULL ; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList ( this, stokens ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */


/*! There are no branches here so just set the AST to Root object everytime.
 * Instantiate with the name from the previous token and zparseStmts()
 */
// Program
ParseResult Parser::parseProgram () {
    ParseResult pr ;
    // Program ::= varName '(' ')' '{' Stmts '}' 
    match(variableName) ;
    string name( prevToken->lexeme ) ;
    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly);
    ParseResult prStmts = parseStmts() ;

    Stmts *ss = dynamic_cast<Stmts *> (prStmts.ast);

    match(rightCurly);
    match(endOfFile) ;
	
	// All match were good to go make root.
	varName *type = new varName(name) ;
	Root *root = new Root(type,ss);
    pr.ast = root;
    return pr ;
}

/*! Their are two branches to account for
 * If the parser finds a '[' character we know to use
 * the long constructor of MatrixDecl
 * Else we we set the AST to the MatrixDecl object with 2 parameters.
 */
// MatrixDecl
// identical purpose of parseDecl, handles special matrix syntax.
ParseResult Parser::parseMatrixDecl () {
    ParseResult pr ;
    match(matrixKwd);
    match(variableName) ;
	 
	 // Save name of var
	 varName *name = new varName( prevToken->lexeme ) ;
    // Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
    if(attemptMatch(leftSquare)){
		
        ParseResult index1 = parseExpr(0);
        Expr *ex1 = dynamic_cast<Expr *> (index1.ast);
        match(comma);
        ParseResult index2 = parseExpr(0);
        Expr *ex2 = dynamic_cast<Expr *> (index2.ast);
        match(rightSquare);
        ParseResult names1 = parseVariableName();
        varName *var1 = dynamic_cast<varName*> (names1.ast);
        match(comma);
        ParseResult names2 =parseVariableName();
        varName *var2 = dynamic_cast<varName*> (names2.ast);
        match(assign);
        ParseResult value = parseExpr(0);
        Expr *val = dynamic_cast<Expr*> (value.ast);
		  
		pr.ast = new MatrixDecl(name,ex1,ex2,var1,var2,val);
		
    }
    // Decl ::= 'Matrix' varName '=' Expr ';'
    else if(attemptMatch(assign)){
		ParseResult value = parseExpr(0);
        Expr* val = dynamic_cast<Expr*>(value.ast);
        pr.ast = new MatrixDecl(name,val);
    }
    else{
        throw ( (string) "Bad Syntax of Matrix Decl in in parseMatrixDecl" ) ;
    }   

    match(semiColon) ;

    return pr ;
}

/*! The if statements will set the type of variable.
 *  Then set the AST to the VarDecl with the type and name.
 */
//standardDecl 
//Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parseStandardDecl(){
    ParseResult pr ;
    
    //ParseResult prType = parseType() ;
	 int type = -1;

    if ( attemptMatch(intKwd) ) {
        type = intKwd;
    } 
    else if ( attemptMatch(floatKwd) ) {
        type = floatKwd;
    }
    else if ( attemptMatch(stringKwd) ) {
        type = stringKwd;
    }
    else if ( attemptMatch(boolKwd) ) {
        type = boolKwd;
    }
	else
	{
		cout<<"Something went wrong here" <<endl;
	}
    match(variableName) ;
	varName *var = new varName( prevToken->lexeme ) ;
	pr.ast = new VarDecl(type,var);
    match(semiColon);
    return pr ;
}

// Decl
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    // Decl :: Matrix variableName ....
    if(nextIs(matrixKwd)){
        pr =parseMatrixDecl();
    } 
    // Decl ::= Type variableName semiColon
    else{
        pr=parseStandardDecl();
    }
    return pr ;
}


/*! There are two branches. A Stmt and subsequent Stmt[s].
 * Or a Stmt and a empty Stmt.
 * We will pass parameters to the StmtsStmts object depending if their are or not.
 */
// Stmts
ParseResult Parser::parseStmts () {
    ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
        // Stmts ::= Stmt Stmts
        ParseResult prStmt = parseStmt() ;
		Stmt *first = dynamic_cast<Stmt*>(prStmt.ast);
        ParseResult prStmts = parseStmts() ;
		Stmts *seq = dynamic_cast<Stmts*>(prStmts.ast);
		pr.ast = new StmtStmts(first,seq);
    }
    else {
        pr.ast = new StmtStmts();
    }
	
    return pr ;
}

/*! Their are 8 main branches for each kind of Stmt.
 * 1. A DeclStmt branch. We set the AST with a DeclStmt the AST returned from parseDecl.
 * 2. BracketStmt branch. This means we set the AST with the AST returned from parseStmts.
 * 3. If it matches logic keywords we set the AST to a LogicGateStmt made from different parse functions.
 * 4. If it matches a variable name then it is a assignment statement and we set the AST to a AssignStmt.
 * 	4a. This is if it matches a matrix then we use constructor with 4 parameters
 *  4b. This is if it matches normal variable then we use the constructor with 2 parameters.
 * 5. A print keyword signifies that we set AST to PrintStmt. PrintStmt is made with what was parsed from parseExpr.
 * 6. Matching the for keyword means we set the AST to ForStmt. ForStmt is made up from multiple parse functions.
 * 7. Matching the while keyword means we set the AST to WhileStmt. WhileStmt is made up from multiple parse functions.
 * 8. Matching a semicolon means we set the AST to SemicolonStmt. Semicolon doesnt require additional parsing.
 */
// Stmt
ParseResult Parser::parseStmt () {
    ParseResult pr ;
	
    //Stmt ::= Decl
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        ParseResult decPr = parseDecl();
		Decl* dec = dynamic_cast<Decl*>(decPr.ast);
		pr.ast = new DeclStmt(dec);

    }
    //Stmt ::= '{' Stmts '}'
    else if (attemptMatch(leftCurly)){
        ParseResult codePr = parseStmts();
		Stmts* code = dynamic_cast<Stmts*>(codePr.ast);
		pr.ast = new BracketStmt(code);
        match(rightCurly);
    }   
    //Stmt ::= 'if' '(' Expr ')' Stmt
    //Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    else if (attemptMatch(ifKwd)){
        match(leftParen);
        ParseResult conditionPr = parseExpr(0);
		Expr* ex = dynamic_cast<Expr*>(conditionPr.ast);
        match(rightParen);
        ParseResult ifCodePr = parseStmt();
		Stmt* stmt = dynamic_cast<Stmt*>(ifCodePr.ast);
		
		LogicGateStmt *logicGate = new LogicGateStmt(ex,stmt);
		  
        if(attemptMatch(elseKwd)){
			ParseResult elseCodePr = parseStmt();
			Stmt* stmt2 = dynamic_cast<Stmt*>(elseCodePr.ast);
			logicGate->elseCode = stmt2;
        }
		  
		pr.ast = logicGate;

    }
    //Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'
    else if  ( attemptMatch (variableName) ) {
		  varName *var = new varName( prevToken->lexeme ) ;
			
		  // messy but works.
        if (attemptMatch ( leftSquare ) ) {
            ParseResult index1Pr = parseExpr(0);
			Expr* index1 = dynamic_cast<Expr*>(index1Pr.ast);
            match ( comma ) ;
            ParseResult index2Pr = parseExpr (0) ;
			Expr* index2 = dynamic_cast<Expr*>(index2Pr.ast);
            match  ( rightSquare ) ;
			match(assign);
			ParseResult valuePr = parseExpr(0);
			Expr* value = dynamic_cast<Expr*>(valuePr.ast);
			match(semiColon);
			pr.ast = new AssignStmt(var,index1,index2,value);
        }
		else
		{
			match(assign);
			ParseResult valuePr = parseExpr(0);
			Expr* value = dynamic_cast<Expr*>(valuePr.ast);
			match(semiColon);
			pr.ast = new AssignStmt(var,value);
		}
		  
		  

    }
    //Stmt ::= 'print' '(' Expr ')' ';'
    else if ( attemptMatch (printKwd) ) {
        match (leftParen) ;
        ParseResult toPrintPr = parseExpr(0);
		Expr* toPrint = dynamic_cast<Expr*>(toPrintPr.ast);
        match (rightParen) ;
        match (semiColon) ;
		pr.ast = new PrintStmt(toPrint);
    }
    //Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
    else if ( attemptMatch (forKwd) ) {
        match (leftParen) ;
        match (variableName) ;
		varName *var = new varName( prevToken->lexeme ) ;
        match (assign) ;
        ParseResult startPr = parseExpr (0);
		Expr* start = dynamic_cast<Expr*>(startPr.ast);
        match (colon) ;
        ParseResult endPr = parseExpr (0) ;
		Expr* end = dynamic_cast<Expr*>(endPr.ast);
        match (rightParen) ;
        ParseResult codePr = parseStmt () ;
		Stmt* code = dynamic_cast<Stmt *>(codePr.ast);
		pr.ast = new ForStmt(var,start,end,code);
    }
    //Stmt ::= 'while' '(' Expr ')' Stmt
    else if (attemptMatch(whileKwd)) {
        match(leftParen);
        ParseResult conditionPr = parseExpr(0);
		Expr* condition = dynamic_cast<Expr*>(conditionPr.ast);
        match(rightParen);
		ParseResult codePr = parseStmt () ;
		Stmt* code = dynamic_cast<Stmt *>(codePr.ast);
		pr.ast = new WhileStmt(condition,code);
    }
    //Stmt ::= ';
    else if ( attemptMatch (semiColon) ) {
        // parsed a skip
		  pr.ast = new SemicolonStmt();
    }
    else{
        throw ( makeErrorMsg ( currToken->terminal ) + " while parsing a statement" ) ;
    }
    // Stmt ::= variableName assign Expr semiColon
    return pr ;
}



// Expr
ParseResult Parser::parseExpr (int rbp) {
    /* Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }

    return left ;
}
/*!Expressions can come in very different shapes and sizes. 
Many Expressions are nested and makes it pertinent to create recursive unparse methods.
there are 11 main branches
1.ExprMatrix
2.ExprBinary
3.ExprIf
4.ExprLet
5.ExprNot
6.ExprParens
7.ExprNestedorFunc
8.ExprIntegerConst
9.ExprFloatConst
10.ExprstringConst
11. varName  
*/

/*
 * parse methods for Expr productions
 * ----------------------------------
 */

 // Expr ::= trueKwd
 ParseResult Parser::parseTrueKwd ( ) {
     ParseResult pr ;
     match ( trueKwd ) ;
     string name( prevToken->lexeme );
     pr.ast = new ExprStringConst(name);
     return pr ;
 }

 // Expr ::= trueKwd
 ParseResult Parser::parseFalseKwd ( ) {
     ParseResult pr ;
     match ( falseKwd ) ;
     string name( prevToken->lexeme ) ;
     pr.ast = new ExprStringConst(name);
     return pr ;
 }


/*! These three below could have been made as 1 class but we decided to make it three just in case. 
The class only outputs strings this makes consts very ambiguous */
// Expr ::= intConst
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
    string name( prevToken->lexeme ) ;
    pr.ast = new ExprStringConst(name);
    
    return pr ;
}

// Expr ::= floatConst
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    string name( prevToken->lexeme ) ;
    pr.ast = new ExprStringConst(name);
    return pr ;
}

// Expr ::= stringConst
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    string name( prevToken->lexeme ) ;
    pr.ast = new ExprStringConst(name);
    return pr ;
}

/*!This will either be a matrix, a nested call, or a simple variable name.
It makes sense to stack the tests this way because they all have variablenames. It allows for stacked decision making. */
// Expr ::= variableName .....
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    varName *var = new varName( prevToken->lexeme ) ;
    if(attemptMatch(leftSquare)){
        ParseResult exp1Pr = parseExpr(0);
		Expr* exp1 = dynamic_cast<Expr*>(exp1Pr.ast);
        match(comma);
        ParseResult exp2Pr = parseExpr(0);
		Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
        match(rightSquare);
        pr.ast = new ExprMatrix(exp1,exp2,var);
        
    }
    //Expr ::= varableName '(' Expr ')'        //NestedOrFunctionCall
    else if(attemptMatch(leftParen)){
        ParseResult exp1Pr = parseExpr(0);
		Expr* exp1 = dynamic_cast<Expr*>(exp1Pr.ast);
        match(rightParen);
        pr.ast = new ExprNestedOrFunc(var,exp1);
        
    }
    //Expr := variableName
    else{
        // variable
        pr.ast = var;
    }
    return pr ;
}




/*! This is just like the nested but does not include a variable name */
// Expr ::= leftParen Expr rightParen
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
    ParseResult exp1Pr = parseExpr(0);
	Expr* exp1 = dynamic_cast<Expr*>(exp1Pr.ast);
    match(rightParen) ;
    pr.ast = new ExprParens(exp1);
    return pr ;
}

//Expr ::= 'if' Expr 'then' Expr 'else' Expr  
ParseResult Parser::parseIfExpr(){  
     ParseResult pr ; 
    
    match(ifKwd);
    ParseResult exp1Pr = parseExpr(0);
	Expr* exp1 = dynamic_cast<Expr*>(exp1Pr.ast);
    match(thenKwd);
    ParseResult exp2Pr = parseExpr(0);
	Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
    match(elseKwd);
    ParseResult exp3Pr = parseExpr(0);
	Expr* exp3 = dynamic_cast<Expr*>(exp3Pr.ast);
    pr.ast = new ExprIf(exp1,exp2,exp3);

    return pr;
}


// Expr ::= 'let' Stmts 'in' Expr 'end' 
ParseResult Parser::parseLetExpr(){
   ParseResult pr ;
   match(letKwd);
   ParseResult prStmts = parseStmts() ;
   Stmts* letStmts = dynamic_cast<Stmts*>(prStmts.ast);
   match(inKwd);
   ParseResult exp1Pr = parseExpr(0);
   Expr* exp1 = dynamic_cast<Expr*>(exp1Pr.ast);
   match(endKwd);
   pr.ast = new ExprLet(letStmts,exp1);

   return pr;
}

// Expr ::= '!' Expr 
ParseResult Parser::parseNotExpr () {
    ParseResult pr ;
    match ( notOp ) ;
    ParseResult exp1Pr = parseExpr(0);
	Expr* exp1 = dynamic_cast<Expr*>(exp1Pr.ast);
    pr.ast = new ExprNot(exp1);
    return pr ;

}

/*! Here is the binary operators. The ones below are all mathematical operators. 
 The only difference in their outputs is the op string.*/
// Expr ::= Expr plusSign Expr
ParseResult Parser::parseAddition ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( plusSign ) ;
    Expr* exp1 = dynamic_cast<Expr*>(prLeft.ast);
    ParseResult exp2Pr = parseExpr(0);
	Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
    pr.ast = new ExprBinary(exp1," + ",exp2);
    return pr ;
}

// Expr ::= Expr star Expr
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( star ) ;
    Expr* exp1 = dynamic_cast<Expr*>(prLeft.ast);
    ParseResult exp2Pr = parseExpr(0);
	Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
    pr.ast = new ExprBinary(exp1," * ",exp2);;
    return pr ;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parseSubtraction ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( dash ) ;
    Expr* exp1 = dynamic_cast<Expr*>(prLeft.ast);
    ParseResult exp2Pr = parseExpr(0);
	Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
    pr.ast = new ExprBinary(exp1," - ",exp2);
    return pr ;
}

// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parseDivision ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( forwardSlash ) ;
    Expr* exp1 = dynamic_cast<Expr*>(prLeft.ast);
    ParseResult exp2Pr = parseExpr(0);
	Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
    pr.ast = new ExprBinary(exp1," / ",exp2);
    return pr;
}


/*! The rest here are relational operators. This depends on only the operators.
   The output will be pretty similar otherwise */

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
ParseResult Parser::parseRelationalExpr ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;

    nextToken( ) ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    string op = prevToken->lexeme ;

    Expr* exp1 = dynamic_cast<Expr*>(prLeft.ast);
    ParseResult exp2Pr = parseExpr(0);
	Expr* exp2 = dynamic_cast<Expr*>(exp2Pr.ast);
    pr.ast = new ExprBinary(exp1,op,exp2);
    return pr ;
}


// Helper function used by the parser.

void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if ( currToken == NULL ) 
        throw ( string("Internal Error: should not call nextToken in unitialized state"));
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

