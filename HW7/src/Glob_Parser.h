#ifndef OTUSHOMEWORKS_GLOB_PARSER_H
#define OTUSHOMEWORKS_GLOB_PARSER_H

#include <iostream>
#include <boost/regex.hpp>

struct Glob_Parser {
public:
    Glob_Parser() = default;
    explicit Glob_Parser(std::string new_str) { parse(std::move(new_str));};
    Glob_Parser(const Glob_Parser &) = default;
    Glob_Parser& operator=(const Glob_Parser &) = default;
    Glob_Parser(Glob_Parser &&) = default;
    Glob_Parser& operator=(Glob_Parser &&) = default;

    inline void set_str(std::string new_str){ parse(std::move(new_str));}

    boost::regex operator()(){ return boost::regex((!expr.empty()) ? expr : ".*");}

private:
    void parse(std::string);

    std::function<void(char)> active_handler;
    void get_sym(char s);
    void get_seq(char s);

    std::string expr;
};


#endif //OTUSHOMEWORKS_GLOB_PARSER_H
