#include "database_schema.hpp"
#include "database.hpp"

static constexpr auto GAMES_TABLE = R"(
CREATE TABLE IF NOT EXISTS Games 
(
    id         INTEGER PRIMARY KEY,
    name       TEXT NOT NULL UNIQUE,    
    genre      TEXT NOT NULL,
    age_rating INTEGER NOT NULL,
    price      REAL NOT NULL,
    copies     INTEGER NOT NULL
))";

static constexpr auto INSERT_GAME = R"(
INSERT OR IGNORE INTO
    Games (name, genre, age_rating, price, copies)      
VALUES
    (?, ?, ?, ?, ?)
)";

namespace 
{
    void addGame(
        Database& database, std::string name, std::string genre, 
        double price, int age_rating, int copies) 
    {
        database.prepare(INSERT_GAME)
            .bindText(name)
            .bindText(genre)
            .bindInteger(age_rating)
            .bindDouble(price)
            .bindInteger(copies)
            .execute();
    }
}

void DatabaseSchema::createTables(Database& database) 
{
    database.prepare(GAMES_TABLE).execute();

    // Populate database with some example games.
    addGame(database, "Fall-Guys"       , "Action"  , 15.99,  3, 300);
    addGame(database, "Flight-Simulator", "Sim"     , 59.99, 3 , 140);
    addGame(database, "Rouge-Legacy-2"  , "Action"  , 15.49, 15, 250);
    addGame(database, "Factorio"        , "Strategy", 21.00, 15, 170);
    addGame(database, "Hades"           , "Action"  , 15.00, 12, 100);
    addGame(database, "Cities-Skylines" , "Strategy", 16.00, 3 , 100);
    addGame(database, "Yakuza-0"        , "Action"  , 14.99, 18, 250);
}
