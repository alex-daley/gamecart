#include "application.h"
#include "command_processor.h"
#include "database.h"
#include "schema.h"

int main()
{
    auto database     = new Database("gamecart.db");
    auto userService  = new UserService(database);
    auto gameService  = new GameService(database);
    auto orderService = new OrderService(database);
    auto commandProcessor         = new CommandProcessor();
    auto application  = new Application(commandProcessor, { gameService, userService, orderService });
    
    Schema::createTables(*database);
    application->run();
    
    delete application;
    delete commandProcessor;
    delete gameService;
    delete userService;
    delete orderService;
    delete database;

    return 0;
}
