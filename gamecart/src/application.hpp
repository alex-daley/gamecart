#pragma once
#include "command_processor.hpp"

class Application {
public:
    Application(CommandProcessor* processor);
    void run();

private:
    CommandProcessor* processor;

    void setup_commands();
    void print_command_help();
    void print_welcome();
};
