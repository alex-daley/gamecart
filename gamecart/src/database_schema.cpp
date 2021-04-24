#include "database_schema.hpp"
#include "database.hpp"

static constexpr auto GAMES_TABLE = R"(
CREATE TABLE IF NOT EXISTS Games (
    id         INTEGER PRIMARY KEY,
    name       TEXT NOT NULL UNIQUE,    
    genre      TEXT NOT NULL,
    price      REAL NOT NULL,
    age_rating INTEGER NOT NULL,
    copies     INTEGER NOT NULL
))";

static constexpr auto INSERT_GAME = R"(
INSERT OR IGNORE INTO
    Games (name, genre, price, age_rating, copies)      
VALUES
    (?, ?, ?, ?, ?)
)";

namespace {
    void insert_game(Database& database, std::string name, std::string genre, double price, int age_rating, int copies) {
        database.prepare(INSERT_GAME)
            .bind_text(name)
            .bind_text(genre)
            .bind_double(price)
            .bind_integer(age_rating)
            .bind_integer(copies)
            .execute();
    }
}

void DatabaseSchema::create_tables(Database& database) {
    database.prepare(GAMES_TABLE).execute();

    // Populate database with some example games.
    insert_game(database, "Fall-Guys"        , "Action"    , 15.99, 3 , 300);
    insert_game(database, "Flight-Simulator" , "Sim"       , 59.99, 3 , 140);
    insert_game(database, "Rouge-Legacy-2"   , "Action"    , 15.49, 15, 250);
    insert_game(database, "Factorio"         , "Strategy"  , 21.00, 15, 170);
    insert_game(database, "Hades"            , "Action"    , 15.00, 12, 100);
    insert_game(database, "Cities-Skylines"  , "Strategy"  , 16.00, 3 , 100);
    insert_game(database, "Yakuza-0"         , "Action"    , 14.99, 18, 250);
    insert_game(database, "Celeste"          , "Platformer", 14.99, 3 , 100);
    insert_game(database, "The Outer Worlds" , "RPG"       , 20.00, 18, 500);
}
