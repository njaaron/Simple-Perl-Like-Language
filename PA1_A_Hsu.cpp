#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "lex.h"

using namespace std;

int main(int argc, char **argv){
    string fileName = "";

    bool vbool = false;

    bool sconst = false;
    vector<string> sconsts;

    bool nconst = false;
    vector<double> nconsts;
    

    bool ident = false;
    vector<string> idents;
    
    for (int i = 1; i < argc; i++){
        string arg = argv[i];
        if(arg.find("-") != -1){
            if (arg == "-v"){
                vbool = true;
            }else if (arg == "-sconst"){
                sconst = true;
            }else if (arg == "-ident"){
                ident = true;
            }else if(arg == "-nconst"){
                nconst = true;
            }else{
                cout << "UNRECOGNIZED FLAG " << "{" << arg << "}" << endl;
                exit(1);
            }
        }else if (fileName == ""){
            fileName = arg;
        }else{
            cout << "ONLY ONE FILE NAME IS ALLOWED." << endl;
        exit(1);
        }
    }
    if(fileName == ""){
        cout << "NO SPECIFIED INPUT FILE." << endl;
        exit(1);
    }
    ifstream openFile(fileName);

    int lineNumber = 0;
    int tokenCount = 0;

    if (!openFile.is_open()){
        cout << "CANNOT OPEN THE FILE " << fileName << endl;
        exit(1);
    }else if(openFile.peek() == std::ifstream::traits_type::eof()){
        cout << "Lines: " << lineNumber << endl;
        cout << "Empty File." << endl;
    }else{
        lineNumber = 1;
        LexItem token;
        while (openFile){
            token = getNextToken(openFile, lineNumber);
            if (token.GetToken() == DONE){
                lineNumber--;
                break;
            }
            else if(token.GetToken() == ERR){
                cout << "Error in line " << token.GetLinenum() << " (" << token.GetLexeme() << ")" << endl;
                exit(1);
            }
            tokenCount++;
            if (vbool){
                cout << token << endl;
            }
            if ( token.GetToken() == SCONST && find(sconsts.begin(), sconsts.end(), token.GetLexeme()) == sconsts.end()){
                sconsts.push_back(token.GetLexeme());
            }
            if (token.GetToken() == ICONST){
                int input = 0;
                if(token.GetLexeme().substr(0,1) == "0"){
                    input = stoi(token.GetLexeme().substr(1,token.GetLexeme().length()));
                }else{
                    input = stoi(token.GetLexeme());
                }
                if(find(nconsts.begin(), nconsts.end(), input) == nconsts.end()){
                    nconsts.push_back(input);
                }
            }
            if ( token.GetToken() == IDENT && find(idents.begin(), idents.end(), token.GetLexeme()) == idents.end()){
                idents.push_back(token.GetLexeme());
            }
            if (  token.GetToken() == NIDENT && find(idents.begin(), idents.end(), token.GetLexeme()) == idents.end()){
                idents.push_back(token.GetLexeme());
            }
            if ( token.GetToken() == SIDENT && find(idents.begin(), idents.end(), token.GetLexeme()) == idents.end()){
                idents.push_back(token.GetLexeme());
            }
            if( token.GetToken() == RCONST){
                double input = stod(token.GetLexeme());
                if(find(nconsts.begin(), nconsts.end(), input) == nconsts.end()){
                    nconsts.push_back(input);
                }
            }
        }
        cout << endl << "Lines: " << lineNumber << endl;
        cout << "Total Tokens: " << tokenCount << endl;
        cout << "Identifiers: " << idents.size() << endl;
        cout << "Numbers: " << nconsts.size() << endl;
        cout << "Strings: " << sconsts.size() << endl;
            
        if (ident == true && idents.size() > 0){
            cout << "IDENTIFIERS:"<<endl;
            sort(idents.begin(), idents.end());
            cout << idents[0] ;
            for(int v = 1; v < idents.size(); v++) 
                cout << ", " << idents[v];
            cout << endl;
        }     
        if (nconst == true && nconsts.size() > 0){
            cout << "NUMBERS:" << endl;
            sort(nconsts.begin(), nconsts.end());
            for(double v : nconsts) 
                cout << v << endl;
        } 
        if(sconst == true && sconsts.size() > 0){
            cout << "STRINGS:" << endl;
            sort(sconsts.begin(), sconsts.end());
            for(string v : sconsts) 
                cout << "\'" << v << "\'" << endl;
        }
    }
    return 0;
}