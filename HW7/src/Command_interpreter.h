#ifndef OTUSHOMEWORKS_COMMAND_INTERPRETER_H
#define OTUSHOMEWORKS_COMMAND_INTERPRETER_H

#include <boost/program_options.hpp>
#include <iostream>
#include "Directory_Compositer.h"
#include "Glob_Parser.h"

struct Command_interpreter {
public:
    Command_interpreter() : m_opt_desc{"Options"} {};
    Command_interpreter(const Command_interpreter &) = delete;
    Command_interpreter & operator=(const Command_interpreter &) = delete;
    Command_interpreter(Command_interpreter &&) noexcept = delete;
    Command_interpreter & operator=(Command_interpreter &&) noexcept = delete;

    boost::program_options::options_description * operator->();
    const boost::program_options::options_description * operator->() const;


    void parse_and_execute(int argc, const char ** argv, Directory_Compositer &) const;
    void parse(int argc, const char ** argv) const;
    void execute(Directory_Compositer &) const;

private:
    mutable boost::program_options::variables_map m_vm;
    boost::program_options::options_description m_opt_desc;
};


#endif //OTUSHOMEWORKS_COMMAND_INTERPRETER_H
