#pragma once

class Database;

namespace DatabaseSchema
{
    // Creates Gamecart database tables if they do not already exist.
    void create_tables(Database& db);
}
