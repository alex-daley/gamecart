#include <optional>
#include <sstream>
#include "application.hpp"
#include "utils.hpp"

namespace
{
    Game parseGame(RowReader& row)
    {
        Game game;
        game.uid       = row.getInteger(0);
        game.name      = row.getText   (1);
        game.genre     = row.getText   (2);
        game.ageRating = row.getInteger(3);
        game.price     = row.getDouble (4);
        game.copies    = row.getInteger(5);
        return game;
    }

    std::optional<Game> findGame(Database& database, const std::string& gameName)
    {
        RowReader row = database.prepare("SELECT * FROM Games where name = ?")
            .bindText(gameName)
            .execute();

        return row
            ? std::optional<Game>(parseGame(row))
            : std::nullopt;
    }

    std::vector<Game> findGames(Database& database)
    {
        RowReader row = database.prepare("SELECT * FROM Games")
            .execute();
        
        if (!row)
            return {};

        std::vector<Game> games;
        do
        {
            Game game = parseGame(row);
            games.push_back(game);
        }
        while (row.step());

        return games;
    }
}

Application::Application(CommandProcessor& proc, Database& database, std::ostream& cout) : 
    proc(proc), 
    database(database), 
    cout(cout)
{
    bindCommands();
}

void Application::run()
{
    printHello();

    while (proc.getIsRunning())
    {
        proc.step();
    }
}

void Application::addToCart(const std::string& gameName)
{
    std::optional<Game> game = findGame(database, gameName);

    if (!game)
    {
        cout << "Game not found\n\n";
    }
    else
    {
        cart.addGame(*game);
        cout << "Game added to cart\n\n";
    }
}

void Application::removeFromCart(const std::string& gameName)
{
    std::optional<Game> game = findGame(database, gameName);

    if (!game)
    {
        cout << "Game not found\n\n";
    }
    else if (!cart.contains(*game))
    {
        cout << "Game not in cart\n\n";
    }
    else
    {
        cart.removeGame(*game);
        cout << "Game removed from cart\n\n";
    }
}

void Application::printCart()
{
    cout << (cart.size() < 1 ? "No games in cart" : cart.prettyPrint()) << "\n\n";
}

void Application::printHello()
{
    std::stringstream s;
    s << "================================================================\n";
    s << " Welcome to the GAMECART command line utility!                  \n";
    s << "================================================================\n";
    s << "Type " << Utils::colour("help") << " to get started           \n\n";
    cout << s.str();
}

void Application::printHelp()
{
    const std::vector<std::string> headings = 
    {
        "Command",  
        "Description" 
    };
    
    std::vector<std::vector<std::string>> rows;
    for (const auto& command : proc.getCommands())
    {
        rows.push_back({ command.name, command.description });
    }

    cout << Utils::formatTable(headings, rows) << "\n\n";
}

void Application::printGames()
{
    std::vector<Game> games = findGames(database);
    const std::vector<std::string> headings =
    {
        "Name", "Genre", "Age Rating", "Price", "Copies"
    };

    std::vector<std::vector<std::string>> rows;
    for (const Game& game : games)
    {
        rows.push_back(game.toStringArray());
    }

    cout << Utils::formatTable(headings, rows) << "\n\n";
}

void Application::bindCommands()
{
    proc.bind(
    {
        "games",
        "View all games available for purchase",
        [&](auto args)
        {
            printGames(); 
        }
    });

    proc.bind(
    {
        "cart show",
        "View games in your cart",
        [&](auto args) 
        {
            printCart(); 
        }
    });

    auto getGameName = [](auto args)
    {
        return args.empty() ? "" : args[0];
    };

    proc.bind(
    {
        "cart add",
        "Add a game to your cart (e.g. cart add Factorio)",
        [&](auto args) 
        { 
            std::string game = getGameName(args);
            addToCart(game);
        }
    });

    proc.bind(
    {
        "cart remove",
        "Remove a game to your cart (e.g. cart remove Factorio)",
        [&](auto args) 
        {
            std::string game = getGameName(args);
            removeFromCart(game);
        }
    });

    proc.bind(
    {
        "help",
        "View command documentation",
        [&](auto args) 
        {
            printHelp();
        }
    });
}
