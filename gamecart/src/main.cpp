#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "command.hpp"
#include "command_processor.hpp"

// Surround a string with the ANSI escape code for yellow.
#define BOLD(str) "\u001b[33m" + std::string(str) + "\u001b[37m"

static void print_command_help(CommandProcessor& processor) {
    std::stringstream help;
    for (auto& command : processor.commands()) {
        help << std::setw(30) << std::left;
        help << BOLD(command.name);
        help << command.description;
        help << std::endl;
    }

    processor.cout() << help.str();
}

CommandProcessor* setup_command_processor() {
    auto processor = new CommandProcessor();

    processor->bind({
        "games",
        "View all games available for purchase",
        [&](auto args) { }
    });

    processor->bind({
        "cart show",
        "View games in your cart",
        [&](auto args) { }
    });

    processor->bind({
        "cart add",
        "Add a game to your cart (e.g. cart add factorio)",
        [&](auto args) { }
    });

    processor->bind({
        "cart remove",
        "Remove a game from your cart (e.g. cart remove factorio)",
        [&](auto args) { }
    });

    processor->bind({
        "cart buy",
        "Purchase games the games in your cart",
        [&](auto args) { }
    });

    processor->bind({
        "q",
        "Quit this utility",
        [&](auto args) { processor->cout() << "Goodbye" << std::endl; }
    });

    processor->bind({
        "help",
        "Output command documentation",
        [&](auto args) { print_command_help(*processor); }
    });

    return processor;
}

int main() {

    CommandProcessor* processor = setup_command_processor();

    processor->cout() << "================================================================\n";
    processor->cout() << " Welcome to GAMECART command line utility!                      \n";
    processor->cout() << "================================================================\n";
    processor->cout() << "Type " BOLD("help") " to get started                          \n\n";

    while (processor->is_running()) {
        processor->step();
    }

    delete processor;

    return 0;
}
