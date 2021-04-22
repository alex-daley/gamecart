#include "database_schema.hpp"
#include "database.hpp"

static constexpr auto USER_TABLE = R"(
CREATE TABLE IF NOT EXISTS User 
(
    id            INTEGER PRIMARY KEY,
    username      TEXT NOT NULL UNIQUE,
    date_of_birth TEXT NOT NULL,
    is_admin      INTEGER NOT NULL
))";


static constexpr auto MAKE_ADMIN = R"((
INSERT OR IGNORE INTO User
    (username, date_of_birth, is_admin)
VALUES
    (?, ?, 1)
))";

void DatabaseSchema::create_tables(Database& database)
{
    database
        .prepare(USER_TABLE)
        .execute();

    database
        .prepare(MAKE_ADMIN)
        .bind_text("admin")
        .bind_text("01/01/1970")
        .execute();
}
 