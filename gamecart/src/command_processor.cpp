#include <iostream>
#include <regex>
#include "command_processor.h"
#include "utils.h"

CommandProcessor::CommandProcessor() 
{
    cin = &std::cin;
}

const std::vector<Command>& CommandProcessor::getCommands() const
{
    return commands;
}

bool CommandProcessor::getIsRunning() const
{
    return isRunning;
}

void CommandProcessor::setCin(std::istream& cin)
{
    this->cin = &cin;
}

void CommandProcessor::bind(const Command& command)
{
    commands.push_back(command);
}

void CommandProcessor::step()
{
    std::string line = readline();
    
    // Ignore if line is empty.
    if (line == "")
        return;

    for (const auto& command : commands)
    {
        std::regex token(command.name);

        // Does the input start with the name of a command?
        if (std::regex_search(line, token))
        {
            // Get command arguments.
            auto args = std::regex_replace(line, token, "");
            auto argArray = Utils::split(args, " ");

            // Invoke callback.
            command.callback(argArray);

            return;
        }
    }

    std::cerr << "Command not found\n";
}

std::string CommandProcessor::readline()
{
    std::string line;
    std::getline(*cin, line);
    return line;
}
