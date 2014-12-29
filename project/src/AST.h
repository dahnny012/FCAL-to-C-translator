#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 
#include "scanner.h"

class varName;

/*! This is the pure virtual class for our AST node.
 * All AST nodes will override the unparse and the cppcod methods.
 * 
 */




// Node
class Node {
	public:
		virtual std::string unparse ( ) { return " This should be pure virtual ";} ;
		virtual std::string cppCode ( ) { return " This should be pure virtual" ;} ;
		virtual ~Node() {};
} ; 


///// Abstracts
class Decl : public Node
{

};


class Program : public Node
{

};

class Stmts : public Node
{


};


class Stmt : public Node
{

};

class Expr : public Node
{

};



/*! This is the concrete class for the Root abstract class.
 *  It is the root of the AST tree.
 */
class Root : public Program
{
	public:
		varName* type;
		Stmts *params;

		/*! This constructor requires 2 parameters, the type and parameters.
		 *\param type refers to the type of function.
		 *\param parameters the function parameters.
		 */
		Root(varName* type , Stmts* parameters);
		std::string cppCode();
		std::string unparse();
};

/*! This is the concrete Stmts class.
 *  It is made for code that is contained in brackets.
 */
 
class StmtStmts : public Stmts
{
	public:
		Stmt *first;
		Stmts *seq;

		std::string cppCode();
		std::string unparse();

		/*! This constructor requires 2 parameters, the first Stmt and the seq of Stmts
		 *\param first refers to the first Stmt
		 *\param seq refers to the next following Stmts.
		 */ 
		StmtStmts(Stmt*,Stmts*);
		StmtStmts();
};


/*! This is a concrete DeclStmt class.
 *  It is used to specify a Decl grammar.
 */
class DeclStmt : public Stmt 
{ 
	public: 
		Decl* dec;
		std::string cppCode(); 
		std::string unparse();
		/*! This constructor requires 1 parameter ,the dec Decl object
		 *\param Dec is a container for the VarDecl.
		 */
		DeclStmt(Decl*);
};

/*! This is a concrete BracketStmt class.
 *  It is when Stmts enclosed by brackets is detected.
 */
class BracketStmt :  public Stmt 
{ 
	public: 
		Stmts *code;
		std::string cppCode(); 
		std::string unparse();
		/*! This constructor requires 1 parameter, the Stmts code.
		 *\param code is the sequence of Stmts inside the brackets.
		 */
		BracketStmt(Stmts*);
};


/*! This is a concrete LogicGateStmt class.
 *  It is used to specify if and ifelse statements
 */
class LogicGateStmt :  public Stmt
{
	public: 
		Expr *condition;
		Stmt *ifCode;
		Stmt *elseCode;
		std::string cppCode(); 
		std::string unparse();
		/*! This constructors requires 2 params for a if grammar 
		 * and 3 params for the ifelse grammar.
		 *\param condition is Expr object for the boolean.
		 *\param ifCode is the code Stmt inside the if block.
		 *\param elseCode is the code Stmt inside the else block.
		 */
		LogicGateStmt(Expr*,Stmt*); // IF
		LogicGateStmt(Expr*,Stmt*,Stmt*); //If Else

};

/*! This is a concrete AssignStmt class.
 * It is used to specify matrixs and scalar variables.
 */
class AssignStmt : public Stmt  
{
	public:
		varName *var;
		int numOfExpr;
		Expr *value;
		Expr *index1;
		Expr *index2;

		std::string cppCode(); 
		std::string unparse();

		/*! There is a constructor for matrix assign and a scalar assign.
		 *\param var is the varName object.
		 *\param value is a Expr that indicates the RValue of the variable.
		 *\param index1 is a Expr that indicates the first index.
		 *\param index2 is a Expr that indicates the second index.  
		 */
		AssignStmt(varName*,Expr* index1,Expr* index2,Expr* value); // matrix assign
		AssignStmt(varName*,Expr* value); // var assign
};

/*! This is a concrete PrintStmt class
 * It is made for a print grammar.
 */

class PrintStmt :  public Stmt 
{ 
	public: 
		Expr *data;
		std::string cppCode(); 
		std::string unparse();
		/*! This constructor takes in 1 Expr.
		 *\param data is the Expr being printed
		 */
		PrintStmt(Expr*);
};



/*! This is a concrete ForStmt class
 * It is made for a for block grammar.
 */
class ForStmt :  public Stmt 
{ 	 
	public: 
	  varName *iter;
	  Expr *start;
	  Expr *end;
	  Stmt *code;
	  std::string cppCode();
	  std::string unparse();
	  /*! This constructor takes 4 parameters, iter,start,end,and code
	   *\param iter is the varName of the iterator.
	   *\param start is a Expr that represents the start value of iter.
	   *\param end is a Expr that represents end condition of iter.
	   *\param code is a Stmt that contains the code being executed in each loop.
	   */
	  ForStmt(varName*,Expr* start,Expr* end,Stmt* code);
};

/*! This is a concrete WhileStmt class.
 * It is made for a while block grammar.
 */
class WhileStmt :  public Stmt 
{ 
	public: 
		Expr *condition;
		Stmt *code;

		std::string cppCode(); 
		std::string unparse();
		/*! This constructor takes 2 parameters 
		 *\param condition is a Expr that represents the boolean in the while.
		 *\param code is a Stmt that contains the code being executed in the loop.
		 */
		WhileStmt(Expr*,Stmt*);
};


/*! This is a concrete SemicolonStmt class. 
 * It is Stmt made for a semicolon grammar.
 */
class SemicolonStmt :  public Stmt 
{ 
	public:
		std::string cppCode(); 
		std::string unparse();
};

/*! This is a concrete VarDecl class.
 * It is a catch all class for Int,std::string,Bool,Float variable declarations.
 */
class VarDecl :  public Decl 
{ 
	public: 
		int type;
		varName *name;
		std::string cppCode(); 
		std::string unparse();
		/*! This constructor the terminal tokenType and the variable name varName.
		 *\param type is a tokenType
		 *\param name is the name of variable.
		 */

		VarDecl(int type ,varName*);

};
/*! This is a concrete MatrixDecl class.
 * It is used to declar Matrix variables.
 */
class MatrixDecl :  public Decl 
{ 
	public: 
		varName *name;
		Expr *index1;
		Expr *index2;
		varName *names1;
		varName *names2;
		Expr *values;
		int type;
		std::string cppCode(); 
		std::string unparse();
		/*! There are is 2 constructors, a short declaration and a long declaration.
		 *\param name is a varName and is used in both and represents the matrix name.
		 *\param index1 is a Expr and represents the row.
		 *\param index2 is a Expr and represents the column.
		 *\param names1 is a varName and represents the x coordinate variable name.
		 *\param names2 is a varName and represents the y coordinate variable name.
		 *\param values is a Expr and represents the Rvalue.
		 */
		MatrixDecl(varName*,Expr* index1,Expr* index2,varName* names1,varName* names2,Expr* values);
		MatrixDecl(varName*,Expr*);
};

/*! ExprMatrix
exp1 is the rows.
exp2 is the cols.
Name is what the name of the matrix is. (The variable name).

The constructor will accept 3 params.
Unparse outputs a matrix in the fcal format. */
class ExprMatrix :  public Expr
{ 	 
	public: 
		Expr* exp1;
		Expr* exp2;
		varName* name;
	std::string cppCode(); 
	std::string unparse();

		ExprMatrix(Expr* exp1, Expr *exp2, varName*);

};
/*! ExprBinary
arg1 is the expression before the operator
arg2 is the expression after the op
op is obviously the operator

the constructor takes in 3 args again and unparse outputs them with the parenthesis
as a matrix
*/
class ExprBinary :  public Expr 
{ 	 
	 public:
		Expr* arg1;
		Expr* arg2;
		std::string op; 
		std::string cppCode(); 
		std::string unparse();
		ExprBinary(Expr* arg1,std::string op,Expr* arg2);
};
/*! varName isused quite a lot in many of our classes
it is the name of expressions and stmts and declarations
var is the actual string
unparse simply returns the string
*/

class varName :  public  Expr
{ 	 
 public: 
	std::string var;

	std::string cppCode(); 
	std::string unparse();
	varName(std::string);
};

/*! We implemented all of the const class the same way.
They all have a string value. 
The constructors will take in a string. 
It will output a string in unparse*/
class ExprIntegerConst :  public Expr 
{ 
	public: 

		std::string value;
		std::string cppCode(); 
	std::string unparse();
		ExprIntegerConst(std::string);
};

/*! We implemented all of the const class the same way.
They all have a string value. 
The constructors will take in a string. 
It will output a string in unparse*/
class ExprFloatConst :  public Expr 
{ 
	public: 

		std::string value;
		std::string cppCode(); 
		std::string unparse();
		ExprFloatConst(std::string);
};

/*! We implemented all of the const class the same way.
They all have a string value. 
The constructors will take in a string. 
It will output a string in unparse*/
class ExprStringConst :  public Expr 
{ 
	public: 

		std::string value;
		std::string cppCode(); 
		std::string unparse();
		ExprStringConst(std::string);
};
/*! ExprNestedorFunc
This takes in the name and a single parameter.
This will not allow for functions with more than one param.
The constructor takes in type varName and Expr. */
class ExprNestedOrFunc : public Expr
{
	public:
 		varName *var;
 		Expr *arg1;
		std::string cppCode();
		std::string unparse();
 		ExprNestedOrFunc(varName*,Expr*);
};

/*!ExprParens
This will put an expression in parenthesis.
It only needs one arg and will output as (arg).
It is used by many different functions  */
class ExprParens : public Expr
{
	public:
		Expr *arg1;
 		std::string cppCode();
		std::string unparse();
 		ExprParens(Expr*);
};
/*! ExprIf 
The if, then, and else varables in the class go after the built in if, then, and else in fcal language.
The constructor will take in the 3 expressions. 
We had to alter the names of the variables otherwise c++ would recognize them as built-ins.
 */
class  ExprIf : public Expr
{
	public:
		Expr *ifCondition;
		Expr *thenCode;
		Expr *elseCode;
		std::string cppCode();
		std::string unparse();
		ExprIf(Expr* ifCondition,Expr *thenCode,Expr *elseCode);
};
/*! ExprLet
This one initializes the stmts class and will use it after the let is instatiated
The constructor will take in 2 params. Unparse outputs in the let format. It will output the proper stmts as they are.
  */
class ExprLet : public Expr
{
	public:
		Stmts *seq;
		Expr *let;
		std::string cppCode();
		std::string unparse();
		ExprLet(Stmts*,Expr*);
};
/*! ExprNot 
It will only take in 1 argument.
Unparse will output the ! symbol before the string.
Arg1 is the expression that is not'ed.
The constructor only needs an expr pointer*/
class ExprNot : public Expr
{
	public:
		Expr* arg1;
		std::string cppCode();
		std::string unparse();
		ExprNot(Expr*);
};






#endif
