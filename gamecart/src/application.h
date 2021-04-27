#pragma once
#include <optional>

#include "command_processor.h"
#include "user_service.h"
#include "game_service.h"
#include "order_service.h"
#include "cart.h"

struct Services
{
    IGameService* games;
    IUserService* users;
    IOrderService* orders;
};

class Application
{
public:
    Application(CommandProcessor* commandProcessor, Services services, std::ostream& cout);
    Application(CommandProcessor* commandProcessor, Services services);
    
    void run();

private:
    bool isLoggedIn() const;
    void login(std::string username, std::string password);
    void logout();

    void logHelp() const;
    void logGames() const;
    void logGames(std::string genre) const;
    void logCart() const;
    void logSales() const;

    void createAccount(std::string username, std::string password, std::string dateOfBirth, std::string email);

    void addToCart(std::string gameName);
    void addToCart(std::string gameName, int quantity);
    void removeFromCart(std::string gameName);
    void buyGamesInCart();

    void bindCommands();

    CommandProcessor* commandProcessor;
    Services services;
    std::ostream* cout;

    std::optional<User> user;
    Cart cart;
};
