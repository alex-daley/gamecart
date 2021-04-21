#include <iostream>
#include <charconv>
#include <string>
#include <optional>
#include <vector>
#include <memory>

#include "application.hpp"
#include "console.hpp"
#include "database.hpp"
#include "database_schema.hpp"

int main() 
{
    Console console;
    std::unique_ptr<Database> database;

    try
    {
        database = std::make_unique<Database>("gamecart.db");
        DatabaseSchema::create_tables(*database);
    }
    catch (const std::runtime_error& err)
    {
        console << "Failed to initialise database\n";
        console << err.what();
        return 1;
    }

    Application app(console, database.get());
    app.run();

    return 0;
} 
