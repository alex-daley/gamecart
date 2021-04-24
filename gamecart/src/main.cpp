#include <memory>
#include <iostream>

#include "application.hpp"
#include "command.hpp"
#include "command_processor.hpp"
#include "database.hpp"
#include "database_schema.hpp"

int main() {
    std::unique_ptr<Database> database;
    try {
        database = std::make_unique<Database>("gamecart.db");
        DatabaseSchema::create_tables(*database);
    }
    catch (const std::runtime_error& err) {
        std::cerr << "Failed to initialise database\n";
        std::cerr << err.what() << std::endl;
        return 1;
    }

    auto processor   = std::make_unique<CommandProcessor>();
    auto application = Application(processor.get(), database.get());
    application.run();
    return 0;
}
