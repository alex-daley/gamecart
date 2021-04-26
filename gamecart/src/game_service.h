#pragma once
#include <string>
#include <vector>
#include "database.h"

struct Game
{
    int uid;
    std::string name;
    std::string genre;
    int ageRating;
    double price;
    int copies;
};

class IGameService
{
public:
    virtual ~IGameService() {}
    
    virtual void insert(const Game& game) = 0;
    virtual void update(const Game& game) = 0;
    
    virtual Game find(std::string name) const = 0;
    virtual Game find(int uid) const = 0;

    virtual std::vector<Game> findAll() const = 0;
    virtual std::vector<Game> findAll(std::string genre) const = 0;
};

class GameService : public IGameService
{
public:
    GameService(Database* database);

    void insert(const Game& game) override;
    void update(const Game& game) override;

    Game find(std::string name) const override;
    Game find(int uid) const override;
    
    std::vector<Game> findAll() const override;
    std::vector<Game> findAll(std::string genre) const override;

private:
    Database* database;
};
