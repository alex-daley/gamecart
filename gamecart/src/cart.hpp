#pragma once
#include <set>
#include "game.hpp"

class Cart
{
public:
    bool contains(const Game& game) const;
    int size() const;

    void addGame(const Game& game);
    void removeGame(const Game& game);

    std::string prettyPrint() const;

private:
    std::set<Game> games;
};
