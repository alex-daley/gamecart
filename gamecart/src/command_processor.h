#pragma once
#include <functional>
#include <istream>
#include <vector>
#include <string>

struct Command
{
    std::string name;
    std::string description;
    std::function<
        void([[maybe_unused]] std::vector<std::string> args)>
        callback;
};

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
