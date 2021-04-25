#include <iostream>

#include "application.hpp"
#include "command_processor.hpp"
#include "database.hpp"
#include "database_schema.hpp"

int main()
{
    CommandProcessor proc;
    Database database("gamecart.db");
    DatabaseSchema::createTables(database);

    Application app(proc, database, std::cout);
    app.run();

    return 0;
}
