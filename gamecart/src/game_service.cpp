#include <stdexcept>
#include "game_service.h"

namespace
{
    Game parseGame(const RowReader& row)
    {
        Game game;
        game.uid       = row.getInteger(0);
        game.name      = row.getText   (1);
        game.genre     = row.getText   (2);
        game.ageRating = row.getInteger(3);
        game.price     = row.getDouble (4);
        game.copies    = row.getInteger(5);
        return game;
    }

    std::vector<Game> parseGames(RowReader& rows)
    {
        std::vector<Game> games;
        do
        {
            games.push_back(parseGame(rows));
        }
        while (rows.step());

        return games;
    }
}

GameService::GameService(Database* database) : database(database)
{
}

void GameService::insert(const Game& game)
{
    static constexpr auto INSERT_GAME = R"(
    INSERT INTO
        Games (name, genre, age_rating, price, copies)      
    VALUES
        (?, ?, ?, ?, ?)
    )";

    database->prepare(INSERT_GAME)
        .bindText(game.name)
        .bindText(game.genre)
        .bindInteger(game.ageRating)
        .bindDouble(game.price)
        .bindInteger(game.copies)
        .execute();
}

void GameService::decrementStock(int uid)
{
    static constexpr auto DECREMENT = R"(
    UPDATE 
        Games 
    SET 
        copies = copies - 1 
    WHERE 
        uid = ? 
        AND copies > 0
    )";

    database->prepare(DECREMENT)
        .bindInteger(uid)
        .execute();
}

Game GameService::find(std::string name) const
{
    static constexpr auto FIND = R"(
    SELECT 
        *
    FROM
        Games 
    WHERE 
        name = ?
    LIMIT
        1
    )";

    auto row = database->prepare(FIND)
        .bindText(name)
        .execute();

    if (!row)
    {
        throw std::runtime_error("Could not find game.");
    }

    return parseGame(row);
}

Game GameService::find(int uid) const
{
    static constexpr auto FIND = R"(
    SELECT 
        *
    FROM
        Games 
    WHERE 
        uid = ?
    LIMIT
        1
    )";

    auto row = database->prepare(FIND)
        .bindInteger(uid)
        .execute();

    if (!row)
    {
        throw std::runtime_error("Could not find game.");
    }

    return parseGame(row);
}

std::vector<Game> GameService::findAll() const
{
    static constexpr auto FIND = R"(
    SELECT
        *
    FROM 
        Games
    ORDER BY
        genre
    )";

    auto row = database->prepare(FIND)
        .execute();

    return parseGames(row);
}

std::vector<Game> GameService::findAll(std::string genre) const
{
    static constexpr auto FIND = R"(
    SELECT 
        * 
    FROM
        Games 
    WHERE 
        genre = ?
    )";

    auto row = database->prepare(FIND)
        .bindText(genre)
        .execute();

    if (!row)
    {
        throw std::runtime_error("Could not find games of genre: " + genre);
    }

    return parseGames(row);
}
