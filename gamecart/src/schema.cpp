#include "schema.h"

static constexpr auto GAMES_TABLE = R"(
CREATE TABLE IF NOT EXISTS Games 
(
    uid        INTEGER PRIMARY KEY,
    name       TEXT NOT NULL UNIQUE,    
    genre      TEXT NOT NULL,
    age_rating INTEGER NOT NULL,
    price      REAL NOT NULL,
    copies     INTEGER NOT NULL
))";

// Passwords really should be hashed :)

static constexpr auto USERS_TABLE = R"(
CREATE TABLE IF NOT EXISTS Users
(
    uid               INTEGER PRIMARY KEY,
    username          TEXT NOT NULL UNIQUE,
    insecure_password TEXT NOT NULL,
    date_of_birth     TEXT NOT NULL,
    email             TEXT NOT NULL
))";

static constexpr auto INSERT_GAME = R"(
INSERT OR IGNORE INTO
    Games (name, genre, age_rating, price, copies)      
VALUES
    (?, ?, ?, ?, ?)
)";

static constexpr auto INSERT_USER = R"(
INSERT OR IGNORE INTO
    Users (username, insecure_password, date_of_birth, email)
VALUES
    (?, ?, ?, ?)
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

    void addUser(
        Database& database, std::string username, std::string insecurePassword,
        std::string dateOfBirth, std::string email)
    {
        database.prepare(INSERT_USER)
            .bindText(username)
            .bindText(insecurePassword)
            .bindText(dateOfBirth)
            .bindText(email)
            .execute();
    }
}

void Schema::createTables(Database& database, bool prepopulate)
{
    database.prepare(GAMES_TABLE).execute();
    database.prepare(USERS_TABLE).execute();


    if (prepopulate)
    {
        // Populate database with some example games.
        addGame(database, "Fall-Guys", "Action", 15.99, 3, 300);
        addGame(database, "Flight-Simulator", "Sim", 59.99, 3, 140);
        addGame(database, "Rouge-Legacy-2", "Action", 15.49, 15, 250);
        addGame(database, "Factorio", "Strategy", 21.00, 15, 170);
        addGame(database, "Hades", "Action", 15.00, 12, 100);
        addGame(database, "Cities-Skylines", "Strategy", 16.00, 3, 100);
        addGame(database, "Yakuza-0", "Action", 14.99, 18, 250);

        // Populate database with some example users.
        addUser(database, "admin", "admin", "01/01/1970", "admin@gamecart.co.uk");
        addUser(database, "john", "doe", "06/08/1997", "john@gamecart.co.uk");
    }
}
