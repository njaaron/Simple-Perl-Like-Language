#include <iostream>
#include <string>
#include <queue> 
#include <map>
#include <iomanip>
#include "val.h"
using namespace std;
Value Value ::operator+(const Value& op) const{
    double answer;
    double op1;
    double op2;
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 + op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 + op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 + op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1 + op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        return Value(GetInt() + op.GetInt());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt() + op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal() + op.GetInt());
    }else{
        return Value();
    }
}
Value Value ::operator-(const Value& op) const{
    double answer;
    double op1;
    double op2;
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 - op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 - op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 - op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1 - op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        return Value(GetInt() - op.GetInt());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt() - op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal() - op.GetInt());
    }else{
        return Value();
    }
}
Value Value ::operator*(const Value& op) const{
    double answer;
    double op1;
    double op2;
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 * op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 * op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 * op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1 * op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        return Value(GetInt() * op.GetInt());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt() * op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal() * op.GetInt());
    }
    else{
        return Value();
    }
}
Value Value:: operator/(const Value& op) const{
    double answer;
    double op1;
    double op2;
    if(IsInt() && op.IsInt()){
        if(op.GetInt() == 0){
            cout << "Run-Time Error-Illegal Division by Zero" << endl;
            return Value();
        }
        return Value(GetInt()/op.GetInt());
    }
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1/op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1/op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1/op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        if(op.GetReal() == 0){
            cout << "Run-Time Error-Illegal Division by Zero" << endl;
            return Value();
        }
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1/op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        if(op.GetInt() == 0){
            cout << "Run-Time Error-Illegal Division by Zero" << endl;
            return Value();
        }
        return Value(GetInt() / op.GetInt());
    }else if(IsInt() && op.IsReal()){
        if(op.GetReal() == 0){
            cout << "Run-Time Error-Illegal Division by Zero" << endl;
            return Value();
        }
        return Value(GetInt() / op.GetReal());
    }else if(IsReal() && op.IsInt()){
        if(op.GetInt() == 0){
            cout << "Run-Time Error-Illegal Division by Zero" << endl;
            return Value();
        }
        return Value(GetReal() / op.GetInt());
    }else{
        return Value();
    }
}
Value Value:: operator==(const Value& op) const{
    bool answer;
    double op1;
    double op2;
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 == op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 == op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1 == op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1 == op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        return Value(GetInt() == op.GetInt());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt() == op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal() == op.GetInt());
    }else{
        return Value();
    }
}
Value Value:: operator>(const Value& op) const{
    bool answer;
    double op1;
    double op2;
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1>op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1>op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1>op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1>op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        return Value(GetInt() > op.GetInt());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt() > op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal() > op.GetInt());
    }else{
        return Value();
    }
}
Value Value:: operator<(const Value& op) const{
    bool answer;
    double op1;
    double op2;
    if(op.GetType() == VSTRING && this->GetType() == VSTRING){
        try{
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1<op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            op1 = op.GetReal();
            op2 = stod(GetString());
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1<op2;
        return Value(answer);
    }else if(op.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op1 = stod(op.GetString());
            op2 = GetReal();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        answer = op1<op2;
        return Value(answer);
    }else if(op.GetType() == VREAL && this->GetType() == VREAL){
        op1 = GetReal();
        op2 = op.GetReal();
        answer = op1<op2;
        return Value(answer);
    }else if(IsInt() && op.IsInt()){
        return Value(GetInt() < op.GetInt());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt() < op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal() < op.GetInt());
    }else{
        return Value();
    }
}
Value Value:: operator^(const Value& oper) const{
    try{
        if(IsInt() && oper.IsInt()){
            double op1 = (double)GetInt();
            double op2 = (double)oper.GetInt();
            double answer = pow(op1, op2);
            return Value(answer);
        }else if(IsInt() && oper.IsReal()){
            double op1 = (double)GetInt();
            double op2 = oper.GetReal();
            double answer = pow(op1, op2);
            return Value(answer);
        }else if(IsReal() && oper.IsInt()){
            double op1 = GetReal();
            double op2 = (double)oper.GetInt();
            double answer = pow(op1, op2);
            return Value(answer);
        }
        double op1 = GetReal();
        double op2 = oper.GetReal();
        double answer = pow(op1, op2);
        return Value(answer);
    }catch(...){
        return Value();
    }
}
Value Value:: Catenate(const Value& oper) const{
    string answer;
    string op1;
    string op2;
    if(oper.GetType() == VSTRING && this->GetType() == VSTRING){
        op1 = GetString();
        op2 = oper.GetString();
        answer = op1 + op2;
        return Value(answer);
    }else if(oper.GetType() == VREAL && this->GetType() == VSTRING){
        try{
            ostringstream outString;
            outString << oper.GetReal();
            op2 = outString.str();
            op1 = GetString();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 + op2;
        return Value(answer);
    }else if(oper.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op2 = oper.GetString();
            ostringstream outStringOther;
            outStringOther << GetReal();
            op1 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 + op2;
        return Value(answer);
    }else if(oper.GetType() == VREAL && this->GetType() == VREAL){
        try{
            ostringstream outString;
            outString << GetReal();
            op1 = outString.str();
            ostringstream outStringOther;
            outStringOther << oper.GetReal();
            op2 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 + op2;
        return Value(answer);
    }else{
        return Value();
    }
}
Value Value::Repeat(const Value& op) const {
    //First operands is a string and the second operand is an integer
    double oper;
    if(IsString() && op.IsString()){
        //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value(newstr);
    }
    else if(IsReal() && op.IsString()){
        //Both operands to be converted
        //first operand must be converted to a string
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        for(int i = 1; i <= times; i++){
            newstr += stroper1;
        }
        return Value(newstr);
    }
    else if (IsReal() && op.IsReal()) {
        //Other cases to follow
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        oper = op.GetReal();
        int times = oper;

        for(int i = 1; i <= times; i++){
            newstr += stroper1;
        }
        return Value(newstr);
    }
    else if (IsString() && op.IsReal()){
        oper = op.GetReal();
        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value(newstr);
    }
    else {
        return Value();
    }
}
Value Value::SEqual(const Value& oper) const{
    string op1;
    string op2;
    bool answer;
    if(oper.GetType() == VSTRING && this->GetType() == VSTRING) {
        op1 = GetString();
        op2 = oper.GetString();
        answer = op1 == op2;
        return Value(answer);
    }else if(oper.GetType() == VREAL && this->GetType() == VSTRING) {
        try{
            ostringstream outString;
            outString << oper.GetReal();
            op2 = outString.str();
            op1 = GetString();
        }catch(const invalid_argument& ia) {
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 == op2; 
        return Value(answer);
    }else if(oper.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op2 = oper.GetString();
            ostringstream outStringOther;
            outStringOther << GetReal();
            op1 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 == op2; 
        return Value(answer);
    }else if (oper.GetType() == VREAL && this->GetType() == VREAL) {
        try{
            ostringstream outString;
            outString << GetReal();
            op1 = outString.str();
            ostringstream outStringOther;
            outStringOther << oper.GetReal();
            op2 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 == op2; 
        return Value(answer);
    }else{
        return Value();
    }
}
Value Value:: SGthan(const Value& oper) const{
    string op1;
    string op2;
    bool answer;
    if(oper.GetType() == VSTRING && this->GetType() == VSTRING) {
        op1 = GetString();
        op2 = oper.GetString();
        answer = op1 > op2;
        return Value(answer);
    }else if(oper.GetType() == VREAL && this->GetType() == VSTRING) {
        try{
            ostringstream outString;
            outString << oper.GetReal();
            op2 = outString.str();
            op1 = GetString();
        }catch(const invalid_argument& ia) {
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 > op2; 
        return Value(answer);
    }else if(oper.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op2 = oper.GetString();
            ostringstream outStringOther;
            outStringOther << GetReal();
            op1 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 > op2; 
        return Value(answer);
    }else if (oper.GetType() == VREAL && this->GetType() == VREAL) {
        try{
            ostringstream outString;
            outString << GetReal();
            op1 = outString.str();
            ostringstream outStringOther;
            outStringOther << oper.GetReal();
            op2 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 > op2; 
        return Value(answer);
    }else{
        return Value();
    }
}
Value Value::SLthan(const Value& oper) const {
    string op1;
    string op2;
    bool answer;
    if(oper.GetType() == VSTRING && this->GetType() == VSTRING) {
        op1 = GetString();
        op2 = oper.GetString();
        answer = op1 < op2;
        return Value(answer);
    }else if(oper.GetType() == VREAL && this->GetType() == VSTRING) {
        try{
            ostringstream outString;
            outString << oper.GetReal();
            op2 = outString.str();
            op1 = GetString();
        }catch(const invalid_argument& ia) {
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 < op2; 
        return Value(answer);
    }else if(oper.GetType() == VSTRING && this->GetType() == VREAL){
        try{
            op2 = oper.GetString();
            ostringstream outStringOther;
            outStringOther << GetReal();
            op1 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 < op2; 
        return Value(answer);
    }else if (oper.GetType() == VREAL && this->GetType() == VREAL) {
        try{
            ostringstream outString;
            outString << GetReal();
            op1 = outString.str();
            ostringstream outStringOther;
            outStringOther << oper.GetReal();
            op2 = outStringOther.str();
        }catch(const invalid_argument& ia){
            cout << "Invalid conversion from double to string." << endl;
            return Value();
        }
        answer = op1 < op2; 
        return Value(answer);
    }else{
        return Value();
    }
}