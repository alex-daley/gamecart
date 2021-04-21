#include <iostream>
#include <charconv>
#include <string>
#include <optional>
#include <vector>
#include "database.hpp"
#include "database_schema.hpp"
#include "console.hpp"

int main() 
{
    try
    {
        Database database("gamecard.db");
        DatabaseSchema::create_tables(database);
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "Fatal Error: Failed to initialise database\n";
        std::cout << err.what();
        return 1;
    }

    Console console;
    int choice = console.menu("Login", "Create Account");

    return 0;
} 
