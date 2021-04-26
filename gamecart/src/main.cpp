#include "application.h"
#include "command_processor.h"
#include "database.h"
#include "schema.h"

int main()
{
    auto database    = new Database("gamecart.db");
    auto userService = new UserService(database);
    auto gameService = new GameService(database); 
    auto proc        = new CommandProcessor();
    auto application = new Application(proc, gameService, userService);
    
    Schema::createTables(*database);
    application->run();

    delete application;
    delete proc;
    delete gameService;
    delete userService;
    delete database;

    return 0;
}
