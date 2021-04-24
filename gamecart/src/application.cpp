#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "application.hpp"
#include "command.hpp"

// Surround a string with the ANSI escape code for yellow.
#define BOLD(str) "\u001b[33m" + std::string(str) + "\u001b[37m"

Application::Application(CommandProcessor* processor, Database* database) : 
    processor(processor), 
    database(database) {
    setup_commands();
}

void Application::run() {
    print_welcome();
    while (processor->is_running()) {
        processor->step();
    }
}

void Application::setup_commands() {
    processor->bind({
        "games",
        "View all games available for purchase",
        [&](auto args) { print_games(); }
    });

    processor->bind({
        "cart show",
        "View games in your cart",
        [&](auto args) { print_cart(); }
    });

    processor->bind({
        "cart add",
        "Add a game to your cart (e.g. cart add factorio)",
        [&](auto args) {
            add_to_cart(args.empty() ? "" : args[0]); 
        }
    });

    processor->bind({
        "cart remove",
        "Remove a game from your cart (e.g. cart remove Factorio)",
        [&](auto args) { 
            remove_from_cart(args.empty() ? "" : args[0]); 
        }
    });

    processor->bind({
        "q",
        "Quit this utility",
        [&](auto args) { 
            processor->cout() << "Goodbye" << std::endl;
        }
    });

    processor->bind({
        "help",
        "Output command documentation",
        [&](auto args) { 
            print_command_help(); 
        }
    });

}

void Application::add_to_cart(std::string game_name) {
    auto rows = database->prepare("SELECT id FROM Games where name = ?")
        .bind_text(game_name)
        .execute();

    if (rows) {
        int id = rows.get_int(0);
        cart.add(id);
        processor->cout() << "Added game: " << game_name << " to cart.\n";
    }
    else {
        std::stringstream info;
        info << "Game: not found\n";
        processor->cout() << info.str();
    }
}

void Application::remove_from_cart(std::string game_name) {
    auto rows = database->prepare("SELECT id FROM Games where name = ?")
        .bind_text(game_name)
        .execute();
    
    if (rows) {
        int id = rows.get_int(0);
        cart.remove(id);
        processor->cout() << "Removed game: " << game_name << " from cart.\n";
    }
    else {
        std::stringstream info;
        info << "Game: not found\n";
        processor->cout() << info.str();
    }
}

void Application::print_command_help() {
    std::stringstream help;
    for (auto& command : processor->commands()) {
        help << std::setw(25) << std::left;
        help << BOLD(command.name);
        help << command.description;
        help << std::endl;
    }

    processor->cout() << help.str();
}

void Application::print_welcome() {
    std::stringstream welcome;
    welcome << "================================================================\n";
    welcome << " Welcome to GAMECART command line utility!                      \n";
    welcome << "================================================================\n";
    welcome << "Type " BOLD("help") " to get started                          \n\n";
    processor->cout() << welcome.str();
}

void Application::print_games() {
    auto rows = database->prepare("SELECT * FROM Games").execute();
    if (!rows) {
        return;
    }

    std::stringstream games;
    games << std::setw(20) << std::left << "Name";
    games << std::setw(10) << std::left << "Genre";
    games << std::setw(10) << std::left << "Price";
    games << std::setw(15) << std::left << "Age Rating";
    games << std::setw(10) << std::left << "Copies";
    games << std::endl;

    do {
        games << std::setw(20) << std::left << rows.get_text(1);
        games << std::setw(10) << std::left << rows.get_text(2);
        games << std::setw(10) << std::left << rows.get_double(3);
        games << std::setw(15) << std::left << rows.get_int(4);
        games << std::setw(10) << std::left << rows.get_int(5);
        games << std::endl;
    }
    while (rows.step());

    processor->cout() << games.str();
}

void Application::print_cart() {
    std::stringstream table;
    table << std::setw(20) << std::left << "Cost (GBP)";
    table << std::setw(20) << std::left << "Name";
    table << std::endl;

    double total_cost = 0.0;

    for (auto& id : cart.game_ids()) {
    
        auto row = database->prepare("SELECT name, price FROM Games WHERE id = ?")
            .bind_integer(id)
            .execute();

        if (row) {
            auto name = row.get_text(0);
            auto cost = row.get_double(1);
            
            table << std::setw(20) << std::left << cost;
            table << std::setw(20) << std::left << name;
            table << std::endl;

            total_cost += cost;
        }
    }

    table << total_cost << std::endl;

    processor->cout() << table.str();
}
