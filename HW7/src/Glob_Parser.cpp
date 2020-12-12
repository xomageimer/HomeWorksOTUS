#include "Glob_Parser.h"

void Glob_Parser::parse(std::string str) {
    if (str.empty())
        return;
    else {
        expr += "(?i)";
        active_handler = std::bind(&Glob_Parser::get_sym, this, std::placeholders::_1);
        for (auto c : str){
            active_handler(c);
        }
    }
}

void Glob_Parser::get_sym(char s) {
    switch (s) {
        case '*' :
            expr += ".*";
            break;
        case '?' :
            expr += ".";
            break;
        case '.' :
            expr += "\\.";
            break;
        default :
            if (s == '[') {
                expr += '(';
                active_handler = std::bind(&Glob_Parser::get_seq, this, std::placeholders::_1);
            }
            expr += s;
            break;
    }
}

void Glob_Parser::get_seq(char s) {
    switch(s) {
        case '-' :
            expr += s;
            break;
        case ']' :
            active_handler = std::bind(&Glob_Parser::get_sym, this, std::placeholders::_1);
            if (ispunct(expr.back()))
                expr.erase(prev(expr.end()));
            expr += s;
            expr += "+";
            expr += ")";
            break;
        default :
            if (!ispunct(expr.back()) && expr.back() != '[') {
                expr += '|';
            }
            expr += s;
            break;
    }
}

