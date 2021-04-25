#pragma once
#include <set>
#include "game.hpp"

class Cart
{
public:
    const std::set<Game>& getGames() const;

    bool contains(const Game& game) const;
    int size() const;

    void addGame(const Game& game);
    void removeGame(const Game& game);
    void clear();

    std::string prettyPrint() const;

private:
    std::set<Game> games;
};
