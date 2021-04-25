#include <vector>
#include "cart.hpp"
#include "utils.hpp"

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

std::string Cart::prettyPrint() const
{
    const std::vector<std::string> headings =
    {
        "Name", "Genre", "Age Rating", "Price", "Copies"
    };

    std::vector<std::vector<std::string>> rows;
    for (const Game& game : games)
    {
        rows.push_back(game.toStringArray());
    }

    return Utils::formatTable(headings, rows);
}
