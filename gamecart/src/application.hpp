#pragma once
#include "cart.hpp"
#include "command_processor.hpp"
#include "database.hpp"
#include "user_manager.hpp"
#include <ostream>

class Application
{
public:
    Application(CommandProcessor& proc, Database& database, std::ostream& cout);
    void run();

private:
    std::ostream& cout;
    CommandProcessor& proc;
    Database& database;
    Cart cart;
    UserManager userManager;

    void addToCart(const std::string& game);
    void removeFromCart(const std::string& game);
    void buyCart();

    void attemptLogin(const LoginRequest& request);
    void logout();

    void printCart();
    void printHello();
    void printHelp();
    void printGames();
    
    void bindCommands();
};
