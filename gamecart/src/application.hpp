#pragma once
#include "command_processor.hpp"
#include "database.hpp"
#include "cart.hpp"

class Application {
public:
    Application(CommandProcessor* processor, Database* database);
    void run();

private:
    CommandProcessor* processor;
    Database* database;
    Cart cart;

    void setup_commands();

    void add_to_cart(std::string game_name);
    void remove_from_cart(std::string game_name);

    void print_command_help();
    void print_welcome();
    void print_games();
    void print_cart();
};
