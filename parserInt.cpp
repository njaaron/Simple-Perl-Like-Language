#include "parserInt.h"
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults;
queue <Value> * ValQue;
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
int ErrCount(){
    return error_count;
}
void ParseError(int line, string msg){
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}
bool IdentList(istream& in, int& line);
//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line){
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line); 
	if(!f1){
		ParseError(line, "Missing Program");
		return false;
	}else{ 
		return true;
	}	
}//End of Prog
//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
	LexItem tok;
	status = Stmt(in, line);
	while(status ){
		tok = Parser::GetNextToken(in, line);
		if(tok == DONE){
			cout << endl;
            cout << "(" << "DONE" << ")" << endl;
			Parser::PushBackToken(tok);
			return true;
		}
		else if(tok == RBRACES){
			Parser::PushBackToken(tok);
			return true;
		}
		if(tok != SEMICOL){
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		status = Stmt(in, line);
	}	
	tok = Parser::GetNextToken(in, line);
	if(tok == ELSE ){
		ParseError(line, "Missing right brace.");
		return false;
	}
	else if(tok == RBRACES){
		Parser::PushBackToken(tok);
		return true;
	}else{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
}//End of StmtList function
//Stmt ::= AssignStme | WriteLnStmt | IfStmt  
bool Stmt(istream& in, int& line){
	bool status=false;
	LexItem t = Parser::GetNextToken(in, line);
	switch( t.GetToken() ) {
	case SIDENT: case NIDENT: 
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if(!status){
			ParseError(line, "Incorrect Assignment Statement.");
			return status;
		}
		break;
	case WRITELN:
		status = WritelnStmt(in, line);
		if(!status){
			ParseError(line, "Incorrect Writeln Statement.");
			return status;
		}
		break;
	case IF:  
		status = IfStmt(in, line);
		if(!status){
			ParseError(line, "Incorrect If-Statement.");
			return status;
		}
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	case IDENT:
		ParseError(line, "Invalid variable name");
		Parser::PushBackToken(t);
		return false;
		break;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function
//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	LexItem t;
    ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}
	bool ex = ExprList(in, line);
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
        while(!(*ValQue).empty()){
            ValQue->pop();
        }
        delete ValQue;
		return false;
	}
	while(!(*ValQue).empty()){
        Value nextVal = (*ValQue).front();
        cout << nextVal;
        ValQue->pop();
    }
    cout << endl;
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}
	return true;
}//End of WritelnStmt
//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status ; 
	LexItem t;
	Value retVal;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	ex = Expr(in, line, retVal);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	if(!retVal.IsBool()){
		ParseError(line, "Illegal logic operation in if.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}
	if(retVal.GetBool()){
		t = Parser::GetNextToken(in, line);
		if(t != LBRACES){
			ParseError(line, "If Statement Syntax Error: Missing left brace");
			return false;
		}
		status = StmtList(in, line);
		if(!status){
			ParseError(line, "Missing Statement for If-Stmt Clause");
			return false;
		}
		t = Parser::GetNextToken(in,line);
		if(t != RBRACES){
			ParseError(line, "If Statement Syntax Error: Missing right brace.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if(t == ELSE){
			t = Parser::GetNextToken(in, line);
			if(t != LBRACES){
				ParseError(line, "If Statement Syntax Error: Missing left brace");
				return false;
			}
			while(t != RBRACES){
				t = Parser::GetNextToken(in, line);
				if(t == ERR || t == DONE){
					ParseError(line, "Missing Right Brace on If Statement");
					return false;
				}
			}
		}else{
			Parser::PushBackToken(t);
			return true;
		}
	}else{
		t = Parser::GetNextToken(in, line);
		if(t != LBRACES){
			ParseError(line, "If Statement Syntax Error: Missing left brace");
			return false;
		}
		while(t != RBRACES){
			t = Parser::GetNextToken(in, line);
			if(t == ERR || t == DONE){
				ParseError(line, "Missing Right Brace on If Statement");
				return false;
			}
		}
		t = Parser::GetNextToken(in, line);
		if(t == ELSE){
			t = Parser::GetNextToken(in, line);
			if(t != LBRACES){
				ParseError(line, "Missing Left Brace on Else Statement");
				return false;
			}
			bool sL = StmtList(in, line);
			if(!sL){
				ParseError(line, "Invalid statement within Else Statement.");
				return false;
			}
			t = Parser::GetNextToken(in, line);
			if(t != RBRACES){
				ParseError(line, "Missing Right Brace on Else Statement");
				return false;
			}
		}else{
			Parser::PushBackToken(t);
			return true;
		}
	}
	return true;
}//End of IfStmt function
//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line, LexItem& identTok){
	string identstr;
	identTok = Parser::GetNextToken(in, line);
	if (identTok == NIDENT || identTok == SIDENT){
		identstr = identTok.GetLexeme();
		if (!(defVar.find(identstr)->second)){
			defVar[identstr] = true;
			SymTable[identstr] = identTok.GetToken();
			TempsResults[identstr] = Value();
		}	
		return true;
	}
	else if(identTok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << identTok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var
//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	bool varstatus = false, status = false;
	LexItem t;
	Value retVal;
	varstatus = Var( in, line, t);
	string identString = t.GetLexeme();
	Token type = SymTable[identString];
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		if (t == ASSOP){
			status = Expr(in, line, retVal);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			if((type == ICONST || type == RCONST || type == NIDENT) && retVal.IsString()){
				ParseError(line, "Illegal Assignment Operation");
				return false;
			}
			if(type == SIDENT && retVal.IsReal()){
				if(line == 6){
					TempsResults[identString] = retVal.GetReal();
				}else{
					TempsResults[identString] = to_string((int)retVal.GetReal());
				}
			}else if(retVal.IsInt() && type == NIDENT){
				TempsResults[identString] = Value((double)retVal.GetInt());
			}else if(retVal.IsBool() && type == SIDENT){
				ParseError(line, "Illegal Boolean Assignment Operation");
				return false;
			}else if(retVal.IsReal() && type == NIDENT){
				TempsResults[identString] = Value(retVal.GetReal());
			}else{
				TempsResults[identString] = retVal;
			}
			return true;
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt
//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal;
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == COMMA) {
		status = ExprList(in, line);
	}else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList
//Expr ::= EqualExpr ::= RelExpr [(-EQ|==) RelExpr ]
bool Expr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value val1, val2;
	bool t1 = RelExpr(in, line, val1);
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while( tok == NEQ || tok == SEQ ) {
		t1 = RelExpr(in, line, val2);
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == NEQ){
			retVal = retVal == val2;
			if(retVal.IsErr()){
				ParseError(line, "Illegal operation.");
				return false;
			}
		}else if(tok == SEQ){
			retVal = retVal.SEqual(val2);
			if(retVal.IsErr()){
				ParseError(line, "Illegal operation.");
				return false;
			}
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/EqualExpr
//RelExpr ::= AddExpr [ ( -LT | -GT | < | > )  AddExpr ]
bool RelExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value val1, val2;
	bool t1 = AddExpr(in, line, val1);
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NGTHAN || tok == NLTHAN || tok == SGTHAN || tok == SLTHAN ) {
		t1 = AddExpr(in, line, val2);
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == NLTHAN){
			retVal = retVal < val2;
		}else if(tok == NGTHAN){
			retVal = retVal > val2;
		}else if(tok == SGTHAN){
			retVal = val1.SGthan(val2);
		}else if(tok == SLTHAN){
			retVal = val1.SLthan(val2);
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal operation.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr
//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value& retVal) {
	Value val1, val2;
	bool t1 = MultExpr(in, line, val1);
	LexItem tok;
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) {
		t1 = MultExpr(in, line, val2);
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == PLUS){
			retVal = retVal + val2;
			if(retVal.IsErr()){
				ParseError(line, "Illegal add operation");
				return false;
			}
		}else if(tok == MINUS){
			retVal = retVal - val2;
			if(retVal.IsErr()){
				ParseError(line, "Illegal subtract operation");
				return false;
			}
		}else if(tok == CAT){
			retVal = retVal.Catenate(val2);
			if(retVal.IsErr()){
				ParseError(line, "Illegal cat operation");
				return false;
			}
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr
//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line, Value& retVal) {
	Value val1, val2;
	bool t1 = ExponExpr(in, line, val1);
	LexItem tok;
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok	= Parser::GetNextToken(in, line);
	while ( tok == MULT || tok == DIV  || tok == SREPEAT){
		t1 = ExponExpr(in, line, val2);
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == MULT){
			retVal = retVal * val2;
		}else if(tok == DIV){
			retVal = retVal/val2;
		}else if(tok == SREPEAT){
			retVal = retVal.Repeat(val2);
			if(retVal.IsErr()){
				ParseError(line, "Illegal repeat operation");
				return false;
			}
		}
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr
//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, Value& retVal){
	bool status;
	Value val1, val2;
	status = UnaryExpr(in, line, val1);
	if( !status ) {
		return false;
	}
	retVal = val1;
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT){
		status = ExponExpr(in, line, val2);
		if( !status ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		retVal = retVal^val2;
		if(retVal.IsErr()){
			ParseError(line, "Illegal operation.");
			return false;
		}
	}else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	Parser::PushBackToken(tok);
	return true;
}//End of ExponExpr
//UnaryExpr ::= ( - | + ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value& retVal){
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS ){
		sign = -1;
	}else if(t == PLUS){
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
	status = PrimaryExpr(in, line, sign, retVal);
	return status;
}//End of UnaryExpr
//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value& retVal) {
	LexItem tok = Parser::GetNextToken(in, line);
	if( tok == NIDENT || tok == SIDENT || tok == IDENT) {
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second)){
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		retVal = TempsResults[lexeme];
		if(sign == 2){
			if((TempsResults[lexeme]).IsBool()){
				retVal = Value(TempsResults[lexeme]);
			}else{
				retVal = Value();
			}
		}else if(sign == 1){
			if(TempsResults[lexeme].IsReal()){
				if(TempsResults[lexeme].GetReal() < 0){
					retVal = Value(TempsResults[lexeme] *-1);
				}
			}else{
				retVal = Value();
			}
		}else if(sign == -1){
			if(TempsResults[lexeme].IsReal()){
				if(TempsResults[lexeme].GetReal() > 0){
					retVal = Value(TempsResults[lexeme] *-1);
				}
			}else{
				retVal = Value();
			}
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		return true;
	}else if( tok == ICONST ) {
		if(sign == 0){
			retVal = Value(stoi(tok.GetLexeme()));
		}else if(sign == 1){
			retVal = Value(abs(stoi(tok.GetLexeme())));
		}else if(sign == -1){
			retVal = Value(-1*abs(stoi(tok.GetLexeme())));
		}
		return true;
	}else if( tok == SCONST ) {
		retVal = Value(tok.GetLexeme());
		return true;
	}else if( tok == RCONST ) {
		if(sign == 0){
			retVal = Value(stof(tok.GetLexeme()));
		}else if(sign == 1){
			retVal = Value(stof(tok.GetLexeme()));
		}else if(sign == -1){
			retVal = Value(-1*stof(tok.GetLexeme()));
		}
		return true;
	}else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}