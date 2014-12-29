#include "AST.h"
#include <ctype.h>
using namespace std;

Root::Root(varName *type,Stmts *parameters)
{
	this->type = type;
	this->params = parameters;
}


string Root::unparse()
{
	
	return type->unparse() + "() {\n"  + params->unparse() + "}";	
}

string Root::cppCode()
{
	return "#include <iostream>\n#include \"Matrix.h\"\n#include <math.h>\n using namespace std;\nint " 
	+ type->cppCode() +
	"() {\n" + 
	params->cppCode() + "}";
}


StmtStmts::StmtStmts(Stmt *first, Stmts *seq)
{
	this->first = first;
	this->seq = seq;
}
StmtStmts::StmtStmts()
{
	this->first = NULL;
	this->seq = NULL;
}

string StmtStmts::unparse()
{
	if(first == NULL)
	{
		return "";	
	}
	
	return first->unparse() + seq->unparse();	
}

string StmtStmts::cppCode()
{
	if(first == NULL)
	{
		return ";";	
	}
	
	return first->cppCode() + seq->cppCode();	
}


DeclStmt::DeclStmt(Decl *dec)
{
	this->dec = dec;
	
}

string DeclStmt::unparse()
{
	return dec->unparse();
}

string DeclStmt::cppCode()
{
	return dec->cppCode();
}


BracketStmt::BracketStmt(Stmts *code)
{
	this->code = code;
}

string BracketStmt::unparse()
{
	return "{\n\t" + code->unparse() + "}\n";
}

string BracketStmt::cppCode()
{
	return "{\n\t" + code->cppCode() + "}\n";
}


LogicGateStmt::LogicGateStmt(Expr *condition , Stmt *ifCode, Stmt *elseCode)
{
	this->condition = condition;
	this->ifCode = ifCode;
	this->elseCode = elseCode;
}

LogicGateStmt::LogicGateStmt(Expr *condition , Stmt *ifCode)
{
	this->condition = condition;
	this->ifCode = ifCode;
}


string LogicGateStmt::unparse()
{
	string temp = "if(\n\t" + condition->unparse()  + ")" + ifCode->unparse();
	if(elseCode != NULL)
	{
		temp.append("else\n\t" + elseCode->unparse());
	}
	
	return temp;
}

string LogicGateStmt::cppCode()
{
	string temp = "if(\n\t" + condition->cppCode()  + ")" + ifCode->cppCode();
	if(elseCode != NULL)
	{
		temp.append("else\n\t" + elseCode->cppCode());
	}
	
	return temp;
}


AssignStmt::AssignStmt(varName *var,Expr *index1,Expr *index2,Expr *value)
{
	numOfExpr = 3;
	this->var = var;
	this->index1 = index1;
	this->index2 = index2;
	this->value = value;
}

AssignStmt::AssignStmt(varName *var,Expr *value)
{
	numOfExpr = 1;
	this->var = var;
	this->value = value;
}


string AssignStmt::unparse()
{
	if(numOfExpr == 1)
	{
		return var->unparse() +  "=" +  value->unparse() + ";\n";
	}
	else
	{
		return var->unparse() + "[" + index1->unparse() + "," + index2->unparse() + "] = \n" +
		value->unparse();
	}
}

string AssignStmt::cppCode()
{
	if(numOfExpr == 1)
	{
		return  var->cppCode() +  "=" +  value->cppCode() + ";\n";
	}
	else
	{
		return "*(" + var->cppCode() +".access(" + index1->cppCode() + "," + index2->cppCode() + ")) = \n" +
		value->cppCode();
	}
}


PrintStmt::PrintStmt(Expr *data)
{
	this->data = data;
}


string PrintStmt::unparse()
{
	return "print(" + data->unparse() + ");\n"; 
}

string PrintStmt::cppCode()
{
	return "cout << " + data->cppCode() + ";\n"; 
}


ForStmt::ForStmt(varName *iter,Expr *start,Expr *end,Stmt *code)
{
	this->iter = iter;
	this->start = start;
	this->end = end;
	this->code = code;
}

string ForStmt::unparse()
{
	return "for(" + iter->unparse() + "=" + start->unparse() + ":" + end->unparse() + ")" +  code->unparse();
}

string ForStmt::cppCode()
{
	string var = iter->cppCode();
	return "for(" + var + "=" + start->cppCode() + ";"+ 
	var + "<=" + end->cppCode()+ ";" + var+"++" + ")" +  code->cppCode();
}


WhileStmt::WhileStmt(Expr *condition,Stmt *code)
{
	this->condition = condition;
	this->code = code;
}

string WhileStmt::unparse()
{
	return "while(" + condition->unparse() +  ")" + code->unparse();
}

string WhileStmt::cppCode()
{
	return "while(" + condition->cppCode() +  ")" + code->cppCode();
}



string SemicolonStmt::unparse()
{
	return ";\n";
}

string SemicolonStmt::cppCode()
{
	return ";\n";
}

VarDecl::VarDecl(int type , varName *name)
{
	this->type = type;
	this->name = name;
}

string VarDecl::unparse()
{
	string temp;
	
	switch (type) 
	{
		case intKwd:
				temp.append("Int ");
				break;
		case floatKwd:
				temp.append("Float ");
				break;
		case boolKwd:
				temp.append("Bool ");
				break;
		case stringKwd:
				temp.append("String ");
		  default:
				return "Something wrong with vardecl unparse";
				break;
	}
	
	return temp + name->unparse() + ";\n";
}

string VarDecl::cppCode()
{
	string temp;
	
	switch (type) 
	{
		case intKwd:
				temp.append("int ");
				break;
		case floatKwd:
				temp.append("float ");
				break;
		case boolKwd:
				temp.append("bool ");
				break;
		case stringKwd:
				temp.append("string ");
		  default:
				return "Something wrong with vardecl unparse";
				break;
	}
	
	return temp + name->cppCode() + ";";
}


string MatrixDecl::unparse(){
	if(type)
	{
		return "Matrix " + name->unparse() + "[" + index1->unparse() + "," + index2->unparse() + "] " + 
		names1->unparse() + ", " + names2->unparse() + " = \n" + values->unparse() + ";";
	}
	else
	{
		return "Matrix " + name->unparse() + " = " + values->unparse() + ";";
	}
}


string MatrixDecl::cppCode(){
	if(type)
	{
		string param1 = names1->cppCode();
		string param2 = names2->cppCode();
		string mName = name->cppCode();
		
		return "Matrix " + mName + "(" + index1->cppCode() + "," + index2->cppCode() + ");\n" +
		"for(int " + param1 + "=0; " + param1 + "< "  + index1->cppCode() + ";" + param1 + "++)\n" +
		"{" + "for(int "+  param2 + "=0;" + param2 + "<" + index2->cppCode() + ";" + param2 + "++)\n" + 
			"{"
				+ "*(" + mName +".access(" + param1 + "," + param2 +"))=" +  values->cppCode() +  
			";}"  +
		"}\n";
		
	}
	else
	{
		return "Matrix " + name->cppCode() + "(" + values->cppCode() + ");";
	}
}


MatrixDecl::MatrixDecl(varName *name, Expr *index1, Expr *index2,varName *names1,varName *names2,Expr *values)
{
	this->name = name;
	this->index1 = index1;
	this->index2 = index2;
	this->names1 = names1;
	this->names2 = names2;
	this->values = values;
	type = 1;
}


MatrixDecl::MatrixDecl(varName *name,Expr *values)
{
	this->name = name;
	this->values = values;
	type = 0;
}

ExprMatrix::ExprMatrix(Expr* exp1, Expr* exp2,varName* name)
{
  this->exp1 = exp1;
  this->exp2 = exp2;
  this->name = name;
}

string ExprMatrix::unparse()
{
 return name->unparse() + "[" + exp1->unparse() + "," + exp2->unparse() + "]";
}

string ExprMatrix::cppCode()
{
 return  "*(" + name->cppCode() + ".access(" + exp1->cppCode() + "," + exp2->cppCode() + "))";
}

ExprBinary::ExprBinary(Expr* arg1,string op,Expr* arg2)
{
 this->arg1 = arg1;
 this->arg2 = arg2;
 this->op = op;
}

string ExprBinary::unparse()
{
 return arg1->unparse() + op + arg2->unparse();
}

string ExprBinary::cppCode()
{
 return arg1->cppCode() + op + arg2->cppCode();
}


varName::varName(string var)
{
	this->var = var;
}

string varName::unparse()
{
	return var;
}

string varName::cppCode()
{
	return var;
}

ExprIntegerConst::ExprIntegerConst(string value)
{
	this->value = value;
}

ExprFloatConst::ExprFloatConst(string value)
{
	this->value = value;
}

ExprStringConst::ExprStringConst(string value)
{
	this->value = value;
}

string ExprIntegerConst::unparse()
{
	return value;
}

string ExprIntegerConst::cppCode()
{
	return value;
}


string ExprFloatConst::unparse()
{
	return value;
}

string ExprFloatConst::cppCode()
{
	return value;
}

string ExprStringConst::unparse()
{
	return value;
}

string ExprStringConst::cppCode()
{
	return value;
}

ExprNestedOrFunc::ExprNestedOrFunc(varName* var,Expr* arg1)
{
	this->var = var;
	this->arg1 = arg1;
}

string ExprNestedOrFunc::unparse()
{
	return var->unparse() + "(" + arg1->unparse() + ")";
}

string ExprNestedOrFunc::cppCode()
{
	if(var->cppCode() == "readMatrix")
	{
		return "Matrix::readMatrix(" + arg1->cppCode() + ")\n";	
	}
	else if(var->cppCode() == "numRows" || var->cppCode() == "numCols")
	{
		return arg1->cppCode() + "." + var->cppCode()+"()";
	}
	else
	{
		return var->cppCode() + "(" + arg1->cppCode() + ")";
	}
		
}


ExprParens::ExprParens(Expr* arg1)
{
	this->arg1 = arg1;
}

string ExprParens::unparse()
{
	return "(" + arg1->unparse() + ")";
}

string ExprParens::cppCode()
{
	return "(" + arg1->cppCode() + ")";
}

ExprIf::ExprIf(Expr* ifCondition,Expr *thenCode,Expr *elseCode)
{
	this->ifCondition = ifCondition;
	this->thenCode = thenCode;
	this->elseCode = elseCode;
}

string ExprIf::unparse()
{
	return "if " + ifCondition->unparse() + "\n then \n\t" + thenCode->unparse() + "\n\telse\n\t" + elseCode->unparse();
}

string ExprIf::cppCode()
{
	return "((" + ifCondition->cppCode() + ")" + "?" + thenCode->cppCode() + " : " + elseCode->cppCode() + ")";
}

ExprLet::ExprLet(Stmts* seq,Expr* let)
{
	this->seq = seq;
	this->let = let;
}

string ExprLet::unparse()
{
	return "let " + seq->unparse() + "in " + let->unparse() + " end"; 
}

string ExprLet::cppCode()
{
	return "({" + seq->cppCode() +  "\n" + let->cppCode() + ";})";
}




ExprNot::ExprNot(Expr* arg1)
{
	this->arg1 = arg1;
}

string ExprNot::unparse()
{
	return "!" + arg1->unparse();
}
string ExprNot::cppCode()
{
	return "!" + arg1->cppCode();
}
