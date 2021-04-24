#pragma once
#include <istream>
#include <ostream>
#include "command.hpp"

class CommandProcessor {
public:
    CommandProcessor(std::istream& cin, std::ostream& cout);
    CommandProcessor();

    bool is_running() const;
    const std::vector<Command>& commands() const;
    std::istream& cin() const;
    std::ostream& cout() const;
    
    void bind(Command command);
    void step();

private:
    std::string readln();
    
    bool is_running_ = true;
    std::vector<Command> commands_;
    std::istream* cin_;
    std::ostream* cout_;
};
