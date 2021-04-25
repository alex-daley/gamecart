#pragma once
#include <istream>
#include <vector>
#include <string>
#include "command.hpp"

class CommandProcessor
{
public:
    CommandProcessor();

    const std::vector<Command>& getCommands() const;
    bool getIsRunning() const;
    void setCin(std::istream& cin);

    void bind(const Command& command);
    void step();

private:
    std::string readline();

    bool isRunning = true;
    std::vector<Command> commands;
    std::istream* cin;
};
