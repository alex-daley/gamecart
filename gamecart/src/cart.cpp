#include <vector>
#include "cart.hpp"
#include "utils.hpp"

const std::set<Game>& Cart::getGames() const
{
    return games;
}

bool Cart::contains(const Game& game) const
{
    return games.find(game) != games.end();
}

int Cart::size() const
{
    return games.size();
}

void Cart::addGame(const Game& game)
{
    games.insert(game);
}

void Cart::removeGame(const Game& game)
{
    games.erase(game);
}

void Cart::clear()
{
    games.clear();
}

std::string Cart::prettyPrint() const
{
    const std::vector<std::string> headings =
    {
        "Name", "Price"
    };

    std::vector<std::vector<std::string>> rows;
    for (const Game& game : games)
    {
        rows.push_back(
        {
            game.name,
            Utils::toDecimalPlaces(game.price, 2)
        });
    }

    return Utils::formatTable(headings, rows);
}
