#include <iostream>
#include <regex>
#include <map>
#include <stdio.h>
#include <ctype.h>
#include "lex.h"

using namespace std;

ostream& operator<<(ostream& out, const LexItem& tok){
    const char* tokens[31] = 
    //Definition of all the possible token types
        // keywords
        {"WRITELN", "IF", "ELSE", 
        // identifiers
        "IDENT", "NIDENT", "SIDENT",

        // an integer, real, and string constant
        "ICONST", "RCONST", "SCONST", 

        // the numeric operators, assignment, numeric and string comparison operators
        "PLUS", "MINUS", "MULT", "DIV", "EXPONENT", "ASSOP", "LPAREN", "RPAREN", "LBRACES", 
        "RBRACES", "NEQ", "NGTHAN", "NLTHAN", "CAT", "SREPEAT", "SEQ",  "SLTHAN", "SGTHAN", 

        //Delimiters
        "COMMA", "SEMICOL", 
        // any error returns this token
        "ERR",

        // when completed (EOF), return this token
        "DONE"};
    string token = tokens[tok.GetToken()];
    out << token;
    bool eval = 
        (tok.GetToken() == ICONST) || (tok.GetToken() == RCONST) || 
        (tok.GetToken() == SCONST) || (tok.GetToken() == IDENT) || (tok.GetToken() == SIDENT) || (tok.GetToken() == NIDENT);
    if (eval){
        out << "(" << tok.GetLexeme() << ")";
    }
    return out;
}

LexItem id_or_kw(const string& lexeme, int linenum){
    map<string,Token> keyWord = {
        {"if", IF},
        {"else",ELSE},
        {"writeln", WRITELN}
    };
    string data = lexeme;
    char firstChar = lexeme[0];
    if(keyWord.find(data) != keyWord.end()){
        return LexItem(keyWord[data], lexeme, linenum);
    }
    if(isalpha(lexeme[0]) || firstChar == '_'){
        return LexItem(IDENT, lexeme, linenum);
    }
    if(firstChar == '$'){
        return LexItem(NIDENT, lexeme, linenum);
    }else if(firstChar == '@'){
        return LexItem(SIDENT, lexeme, linenum);
    }
    return LexItem(ERR, "", linenum);
}


LexItem getNextToken(istream& in, int& linenum){
    enum TokState {START, INID, INCOMMENT, INSTRING, ININT, INREAL, MINUS_STATE}lexstate = START;
    string lexeme = "";
    char ch;
    char nChar;
    string line;
    while(in.get(ch)){
        nChar = in.peek();
        switch(lexstate){
        case START:
            if(ch == '#'){
                lexstate = INCOMMENT;
                continue;
            }
            else if(ch == '\''){
                lexstate = INSTRING;
                continue;
            }
            if(ch == '\n'){
                linenum++;
            }
            if(isspace(ch)){
                continue;
            }
            lexeme = ch;
            if(lexeme == "+"){
                return LexItem(PLUS, lexeme, linenum);
            }else if(lexeme == "*"){
                if(nChar == '*'){
                    in.get();
                    return LexItem(SREPEAT, lexeme+nChar, linenum);
                }
                else{
                    return LexItem(MULT, lexeme, linenum);
                }
            }else if(lexeme == "/"){
                return LexItem(DIV, lexeme, linenum);
            }else if(lexeme == "="){
                if(in.peek() == '='){
                    in.get(ch);
                    lexeme += ch;
                    return LexItem(NEQ, lexeme, linenum);
                }
                else{
                    return LexItem(ASSOP, lexeme+ch, linenum);
                }
            }else if(lexeme == "("){
                return LexItem(LPAREN, lexeme, linenum);
            }else if(lexeme == ")"){
                return LexItem(RPAREN, lexeme, linenum);
            }else if(lexeme == ">"){
                return LexItem(NGTHAN, lexeme, linenum);
            }else if(lexeme == "<"){
                return LexItem(NLTHAN, lexeme, linenum);
            }else if(lexeme == ";"){
                return LexItem(SEMICOL, lexeme, linenum);
            }else if(lexeme == ","){
                return LexItem(COMMA, lexeme, linenum);
            }else if(lexeme == "{"){
                return LexItem(LBRACES, lexeme, linenum);
            }else if(lexeme == "}"){
                return LexItem(RBRACES, lexeme, linenum);
            }else if(lexeme == "^"){
                return LexItem(EXPONENT, lexeme, linenum);
            }else if(lexeme == "."){
                return LexItem(CAT, lexeme, linenum);
            }else if(lexeme == "-"){
                lexstate = MINUS_STATE;
            }
            else if(isdigit(ch)){
                lexstate = ININT;
                continue;
            }
            else if(lexeme == "."){
                lexstate = INREAL;
                continue;
            }
            else if(isalpha(ch) || ch == '_' || ch == '$' || ch == '@'){
                lexstate = INID;
                continue;
            }
            else{
                return LexItem(ERR, lexeme, linenum);
            }
            break;
        case INID:
            if(isalnum(ch) || ch == '_'){
                lexeme += ch;
                continue;
            }
            else {
                in.putback(ch);
                return id_or_kw(lexeme, linenum);
            }
            break;
        case INCOMMENT:
            if(ch == '\n'){
                linenum++;
                lexstate = START;
            }
            break;
        case INSTRING:
            if (ch == '\''){
                return LexItem(SCONST, lexeme, linenum);
            }else if(ch == '\n'){
                return LexItem(ERR, "\'" + lexeme, linenum);
            }else{
                lexeme += ch;
            }
            break;
        case ININT:
            if(isdigit(ch)){
                lexeme += ch;
            }else if(ch == '.'){
                lexstate = INREAL;
                lexeme += ch;
                continue;
            }else{
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }
            break;
        case INREAL:
            if(isdigit(ch)){
                lexeme += ch;
            }
            else if(ch == '.'){
                lexeme += ch;
                return LexItem(ERR, lexeme, linenum);
            }
            else{
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
            }
            break;
        case MINUS_STATE:
            if(tolower(ch) == 'e' || tolower(ch) == 'l' || tolower(ch) == 'g'){
                if(tolower(ch) == 'e' && tolower(nChar) == 'q'){
                    in.get();
                    return LexItem(SEQ, lexeme+ch+nChar, linenum);
                }
                else if(tolower(ch) == 'l' && tolower(nChar) == 't'){
                    in.get();
                    return LexItem(SLTHAN, lexeme+ch+nChar, linenum);
                }
                else if(tolower(ch) == 'g' && tolower(nChar) == 't'){
                    in.get();
                    return LexItem(SGTHAN, lexeme+ch+nChar, linenum);
                }
                else{
                    return LexItem(MINUS, lexeme, linenum);
                }
            }
            else{
                in.putback(ch);
                return LexItem(MINUS, lexeme, linenum);
            }
            break;
        }
    }
    return LexItem(DONE, lexeme, linenum);
}