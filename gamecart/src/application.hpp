#pragma once
#include <string>
#include "console.hpp"
#include "database.hpp"

class Application
{
public:
    Application(Console console, Database* database);
    void run();

private:
    void create_account_dialog();
    void login_dialog();

    bool username_exists(std::string username) const;

    Console console;
    Database* database;
};
