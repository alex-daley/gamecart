#pragma once
#include "command_processor.hpp"
#include "database.hpp"

class Application {
public:
    Application(CommandProcessor* processor, Database* database);
    void run();

private:
    CommandProcessor* processor;
    Database* database;

    void setup_commands();
    void print_command_help();
    void print_welcome();
    void print_games();
};
