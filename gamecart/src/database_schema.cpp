#include "database_schema.hpp"
#include "database.hpp"

static constexpr auto USER_TABLE = R"(
CREATE TABLE IF NOT EXISTS User 
(
    id            INTEGER PRIMARY KEY,
    username      TEXT NOT NULL,
    date_of_birth TEXT NOT NULL,
    is_admin      INTEGER NOT NULL
))";

void DatabaseSchema::create_tables(Database& db)
{
    db.prepare(USER_TABLE).execute();
}
