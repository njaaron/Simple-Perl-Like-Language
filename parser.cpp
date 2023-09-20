/* Implementation of Recursive-Descent Parser
 * parser.cpp
 * Programming Assignment 2
 * Spring 2023
*/

#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}
bool Prog(istream& in, int& line){
	bool status = true;
	status = StmtList(in, line);
	if(!status){
		ParseError(line, "Missing Program");
		return false;
	}
	cout << "(DONE)" << endl;
	return status;
}
bool StmtList(istream& in, int& line){
	bool status = Stmt(in, line);
	if(!status){
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	if(t != SEMICOL){
		ParseError(line, "Missing semicolon at end of Statement.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t == DONE || t == RBRACES){
		Parser::PushBackToken(t);
		return true;
	}else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	Parser::PushBackToken(t);
	return StmtList(in, line);
}
bool Stmt(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok == NIDENT || tok == SIDENT){
        Parser::PushBackToken(tok);
		if(!AssignStmt(in, line)){
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
    }else if (tok == WRITELN){
		if(!WritelnStmt(in, line)){
			ParseError(line, "Incorrect Writeln Statement");
			return false;
		}
    }else if (tok == IF){
		if(!IfStmt(in, line)){
			ParseError(line, "Incorrect If-Statement.");
			return false;
		}
    }else{
        return false;
    }
	return true;
}

//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in Writeln Stmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return true;
}//End of WritelnStmt
//IfStmt:= if (Expr) '{' StmtList '}' [ else '{' StmtList '}' ]
bool IfStmt(istream& in, int& line) {
    LexItem t;
    bool status;
    t = Parser::GetNextToken(in, line);
    if(t != LPAREN) {
        ParseError(line, "Missing Left Parenthesis of If Statement");
        return false;
    }
    // Parse the expression in the if condition
    status = Expr(in, line);
    if(!status){
        ParseError(line, "Missing Expression in If Statement");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if( t != RPAREN ){
        ParseError(line, "Missing Right Parenthesis of If Statement");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if( t != LBRACES ){
        ParseError(line, "Missing Left Brace of If Statement");
        return false;
    }
	status = StmtList(in, line);
    if(!status){
        ParseError(line, "Invalid Statements in If Statement");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t != RBRACES){
        ParseError(line, "Missing Right Brace of If Statement");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t == ELSE){
        t = Parser::GetNextToken(in, line);
        if(t != LBRACES){
            ParseError(line, "Missing Left Brace of Else Statement");
            return false;
        }
        status = StmtList(in, line);
        if(!status){
            ParseError(line, "Invalid Statements in Else Statement");
            return false;
        }
        t = Parser::GetNextToken(in, line);
        if(t != RBRACES) {
            ParseError(line, "Missing Right Brace of Else Statement");
            return false;
        }
    }else{
        Parser::PushBackToken(t);
	}
    return true;
}
bool AssignStmt(istream& in, int& line){
    bool status = Var(in, line);
    if(!status){
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }
    LexItem t = Parser::GetNextToken(in, line);
	string lexeme = t.GetLexeme();
	t = Parser::GetNextToken(in, line);
    if(t != ASSOP){
		if(t == SEMICOL){
			defVar[lexeme] = false;
			Parser::PushBackToken(t);
			status = true;
		}else{
			ParseError(line, "Missing assignment op");
			status = false;
		}
        return status;
    }
    status = Expr(in, line);
    if (!status){
        ParseError(line, "Missing Expression in Assign Statement");
        return false;
    }
	defVar[lexeme] = true;
    return true;
}
// Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);
	Parser::PushBackToken(tok);
    if (tok == NIDENT || tok == SIDENT){
        return true;
    }else{
        ParseError(line, "Invalid Variable Name");
        return false;
    }
}
//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList
bool Expr(istream& in, int& line){
    bool status = false;
    status = RelExpr(in, line);
    if (!status){
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok == SEQ || tok == NEQ) {
        status = RelExpr(in, line);
        if (!status){
            ParseError(line, "Missing opperand after operator");
            return false;
        }
    }else{
        Parser::PushBackToken(tok);
    }
    return true;
}
bool RelExpr(istream& in, int& line){
    bool status = AddExpr(in, line);
    if (!status){
        return false;
    }
    LexItem t = Parser::GetNextToken(in, line);
    if (t == SLTHAN || t == SGTHAN || t == NLTHAN || t == NGTHAN) {
        status = AddExpr(in, line);
        if (!status){
            ParseError(line, "Missing right operand after relational operator");
            return false;
        }
    }else{
		Parser::PushBackToken(t);
	}
    return true;
}
//AddExpr ::= MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line) {
    bool status = MultExpr(in, line);
    if (!status) {
        return false;
    }
    LexItem t = Parser::GetNextToken(in, line);
    while (t == PLUS || t == MINUS || t == CAT){
        status = MultExpr(in, line);
        if (!status) {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        t = Parser::GetNextToken(in, line);
    }
    Parser::PushBackToken(t);
    return true;
}
// MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line) {
    bool status = ExponExpr(in, line);
    if (!status){
        return false;
    }
    LexItem t = Parser::GetNextToken(in, line);
    while (t == MULT || t == DIV || t == SREPEAT){
        status = ExponExpr(in, line);
        if (!status){
            ParseError(line, "Missing expression after operator");
            return false;
        }
        t = Parser::GetNextToken(in, line);
    }
    Parser::PushBackToken(t);
    return true;
}
// ExponExpr ::= UnaryExpr { ^ UnaryExpr }
bool ExponExpr(istream& in, int& line) {
    bool status = UnaryExpr(in, line);
    if (!status){
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    while (tok == EXPONENT) {
        status = UnaryExpr(in, line);
        if (!status) {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
    }
    Parser::PushBackToken(tok);
    return true;
}
// UnaryExpr ::= [(-|+)] PrimaryExpr
bool UnaryExpr(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if (t == PLUS){
        return PrimaryExpr(in, line, 1);
    }else if(t == MINUS){
		return PrimaryExpr(in, line, -1);
	}
    Parser::PushBackToken(t);
    return PrimaryExpr(in, line, 1);
}
// PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign) {
    LexItem tok = Parser::GetNextToken(in, line);
	bool status = false;
	if(tok == SIDENT || tok == NIDENT){
		if(defVar[tok.GetLexeme()] == false){
			ParseError(line, "Undeclared variable");
			return false;
		}
		status = true;
	}else if(tok == IDENT || tok == ICONST || tok == RCONST || tok == SCONST){
		status = true;
	}else if (tok == LPAREN){
		status = Expr(in, line);
		if(!status){
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if(Parser::GetNextToken(in, line) == RPAREN){
			return status;
		}else{
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
    }
    return status;
}