#include <iostream>
#include "database.hpp"
#include "database_schema.hpp"

int main() 
{
    try
    {
        Database db("gamecard.db");
        DatabaseSchema::create_tables(db);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Fatal Error: Failed to initialise database\n";
        std::cout << e.what();
        return 1;
    }

    return 0;
} 
 