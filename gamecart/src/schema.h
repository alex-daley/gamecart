#pragma once
#include "database.h"

namespace Schema
{
    // Create Gamecart database tables if they do not already exist.
    void createTables(Database& database);
}
