#pragma once
#include <optional>

#include "command_processor.h"
#include "user_service.h"
#include "game_service.h"
#include "cart.h"

class Application
{
public:
    Application(CommandProcessor* proc, IGameService* games, IUserService* users, std::ostream& cout);
    Application(CommandProcessor* proc, IGameService* games, IUserService* users);
    
    void run();

private:
    bool isLoggedIn() const;
    void login(std::string username, std::string password);
    void logout();

    void logHelp() const;
    void logGames() const;
    void logGames(std::string genre) const;
    void logCart() const;

    void addToCart(std::string gameName);
    void addToCart(std::string gameName, int quantity);
    void removeFromCart(std::string gameName);
    void buyGamesInCart();

    void bindCommands();

    CommandProcessor* proc;
    IGameService* games;
    IUserService* users;
    std::ostream* cout;

    std::optional<User> user;
    Cart cart;
};
