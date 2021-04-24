#include <iostream>
#include <regex>
#include <vector>
#include "command_processor.hpp"

namespace {

    // Takes a sentance and returns each word as an array.
    // For example: "blah blah" => ["blah", "blah"]
    std::vector<std::string> split_on_whitespace(std::string str) {
        std::vector<std::string> fragments;
        
        int last_position = 0;
        while ((last_position = str.find(" ")) != std::string::npos) {
            std::string fragment = str.substr(0, last_position);
            if (fragment != "") 
                fragments.push_back(fragment);
            str.erase(0, last_position + 1);
        }

        fragments.push_back(str);

        return fragments;
    }
}

CommandProcessor::CommandProcessor() : 
    cin_(&std::cin), cout_(&std::cout) {
}

bool CommandProcessor::is_running() const {
    return is_running_;
}

const std::vector<Command>& CommandProcessor::commands() const {
    return commands_;
}

std::istream& CommandProcessor::cin() const {
    return *cin_;
}

std::ostream& CommandProcessor::cout() const {
    return *cout_;
}

void CommandProcessor::step() {
    std::string line = readln();

    for (auto& command : commands_) {
        std::regex name(command.name);
        
        // Does the input contain the name of a command?
        if (std::regex_search(line, name)) {

            // Parse the argumetns (any words after the command's name)
            auto all_args = std::regex_replace(line, name, "");
            auto args = split_on_whitespace(all_args);    
            
            // Invoke the command callback and pass in parsed arguments.
            command.callback(args);
            cout() << std::endl;
            return;
        }
    }

    cout() << "Command not found.\n";
}

std::string CommandProcessor::readln() {
    std::string line;
    std::getline(*cin_, line);
    return line;
}

CommandProcessor::CommandProcessor(std::istream& cin, std::ostream& cout) : 
    cin_(&cin), cout_(&cout) {
}

void CommandProcessor::bind(Command command) {
    commands_.push_back(command);
}
