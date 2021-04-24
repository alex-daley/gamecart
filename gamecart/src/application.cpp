#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "application.hpp"
#include "command.hpp"

// Surround a string with the ANSI escape code for yellow.
#define BOLD(str) "\u001b[33m" + std::string(str) + "\u001b[37m"

Application::Application(CommandProcessor* processor) : 
    processor(processor){
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
        [&](auto args) {}
    });

    processor->bind({
        "cart show",
        "View games in your cart",
        [&](auto args) {}
    });

    processor->bind({
        "cart add",
        "Add a game to your cart (e.g. cart add factorio)",
        [&](auto args) {}
    });

    processor->bind({
        "cart remove",
        "Remove a game from your cart (e.g. cart remove factorio)",
        [&](auto args) {}
    });

    processor->bind({
        "cart buy",
        "Purchase games the games in your cart",
        [&](auto args) {}
    });

    processor->bind({
        "q",
        "Quit this utility",
        [&](auto args) { processor->cout() << "Goodbye" << std::endl; }
    });

    processor->bind({
        "help",
        "Output command documentation",
        [&](auto args) { print_command_help(); }
    });

}

void Application::print_command_help() {
    std::stringstream help;
    for (auto& command : processor->commands()) {
        help << std::setw(30) << std::left;
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
