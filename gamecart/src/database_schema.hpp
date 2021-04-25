#pragma once
#include "database.hpp"

namespace DatabaseSchema 
{
    // Creates Gamecart database tables if they do not already exist.
    void createTables(Database& database);
}
