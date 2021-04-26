#include <sstream>
#include <iostream>
#include <stdexcept>
#include "application.h"
#include "utils.h"

namespace
{
    using Row  = std::vector<std::string>;
    using Rows = std::vector<Row>;
    
    const std::vector<std::string> gameHeadings = 
    { 
        "name", "genre", "age rating", "price", "copies" 
    };

    Row toRow(const Game& game)
    {
        return 
        {
            game.name,
            game.genre,
            std::to_string(game.ageRating) + "+",
            Utils::toDecimalPlaces(game.price, 2),
            std::to_string(game.copies)
        };
    }
}

Application::Application(CommandProcessor* proc, IGameService* games, IUserService* users, std::ostream& cout) :
    proc(proc),
    games(games),
    users(users),
    cout(&cout)
{
    bindCommands();
}

Application::Application(CommandProcessor* proc, IGameService* games, IUserService* users) :
    Application(proc, games, users, std::cout)
{
}

void Application::run()
{
    std::stringstream s;
    s << "====================================================================\n";
    s << " Welcome to the GAMECART command line utility!             \n";
    s << "====================================================================\n";
    s << "Type " << Utils::colour("help") << " to get started\n";
    *cout << s.str();

    while (proc->getIsRunning())
    {
        *cout << "\n";
        
        // Listen for new input commands.
        proc->step();
    }
}

bool Application::isLoggedIn() const
{
    return (bool)user;
}

void Application::login(std::string username, std::string password)
{
    if (isLoggedIn())
    {
        *cout << "You are already logged in\n";
        return;
    }

    try
    {
        this->user = users->login({ username, password });
        *cout << "Logged in as: " << username << "\n";
    }
    catch (const std::runtime_error& e)
    {
        *cout << "Failed to log in: " << e.what() << "\n";
    }
}

void Application::logout()
{
    if (!isLoggedIn())
    {
        *cout << "You are not logged in\n";
    }
    else if (cart.getIsEmpty())
    {
        *cout << "Your cart is not empty.\nPlease purchase or clear you cart before logging out\n";
    }
    else
    {
        user = std::nullopt;
        *cout << "User logged out\n";
    }
}

void Application::logHelp() const
{
    Rows rows;
    for (auto& [name, help, _] : proc->getCommands())
        rows.push_back({ Utils::colour(name), help });
    
    auto table = Utils::formatTable({}, rows);
    *cout << table << "\n";
}

void Application::logGames() const
{
    logGames("*");
}

void Application::logGames(std::string genre) const
{
    auto filteredGames = (genre == "*") 
        ? games->findAll() 
        : games->findAll(genre);

    Rows rows;
    for (const auto& game : filteredGames)
        rows.push_back(toRow(game));

    auto table = Utils::formatTable(gameHeadings, rows);
    *cout << table << "\n";
}

void Application::logCart() const
{
    if (cart.getIsEmpty())
    {
        *cout << "There are no games in your cart.\n";
        return;
    }

    double totalCost = 0.0;

    Rows rows;
    for (const auto& [gameID, quantity] : cart.getOrders())
    {
        Game game = games->find(gameID);
        double cost = game.price * quantity;
        totalCost += cost;

        rows.push_back({ 
            game.name, 
            Utils::toDecimalPlaces(cost, 2),
            std::to_string(quantity)
        });
    }

    auto table = Utils::formatTable({ "Name", "Cost", "Quantity" }, rows);
    *cout << table << "\n";

    *cout << "Total cost: " << Utils::toDecimalPlaces(totalCost, 2) << "\n";
    *cout << "Total cost: " << Utils::toDecimalPlaces(totalCost * 0.8, 2) << " (before 20% VAT)\n";
}

void Application::addToCart(std::string gameName)
{
    addToCart(gameName, 1);
}

void Application::addToCart(std::string gameName, int quantity)
{
    // So we can rollback if an insertion fails.
    [[maybe_unused]]
    Cart prevCart = cart; 

    try
    {
        Game game = games->find(gameName);
        int copiesInCart = cart.getCount(game.uid);

        if (game.copies - copiesInCart < quantity)
        {
            *cout << "There are only " << game.copies - copiesInCart << " copies left in stock; please revise your order quantity\n";
        }
        else
        {
            cart.addOrder(game.uid, quantity);
            *cout << "Added " << game.name << " (x" << quantity << ") to cart\n";
        }
    }
    catch (const std::runtime_error* e)
    {
        *cout << "Could not add order to cart: " << e->what() << "\n";
        cart = prevCart;
    }
}

void Application::removeFromCart(std::string gameName)
{
    try
    {
        Game game = games->find(gameName);

        if (cart[game.uid] == 0)
        {
            *cout << "Game is not in you cart\n";
        }
        else
        {
            cart.removeAt(game.uid);
            *cout << "Removed game from your cart\n";
        }
    }
    catch (...)
    {
        *cout << "Game not found\n";
    }
}

void Application::buyGamesInCart()
{
    if (!isLoggedIn())
    {
        *cout << "Please log in before buying the items in your cart\n";
        return;
    }

    logCart();
    *cout << "Enter y to confirm order\n";

    std::string input;
    std::cin >> input;
    if (!(input == "y" || input == "Y"))
    {
        *cout << "Order cancelled\n";
        return;
    }

    for (auto& [gameID, quantity] : cart.getOrders())
    {
        for (int i = 0; i < quantity; i++)
            games->decrementStock(gameID);
    }

    cart.clear();
}

void Application::bindCommands()
{
    proc->bind({
        "games",
        "Display a list of games available to purchase. Optionally filter by genre",
        [this](auto args)  {
            if (args.size() > 0)
                logGames(args[0]);
            else
                logGames();
        }
    });

    proc->bind({
        "cart add",
        "Add a game to your cart. Optionally specify a quantity",
        [this](auto args) 
        {
            if (args.size() < 1)
            {
                *cout << "Please specify the name of the game you would like to purchase\n";
            }
            else
            {
                std::string game = args[0];
                if (args.size() == 2)
                {
                    std::optional<int> quantity = Utils::parseInt(args[1]);
                    if (!quantity)
                        *cout << "Invalid quantity value\n";
                    else
                        addToCart(game, *quantity);
                }
                else
                {
                    addToCart(game);
                }
            }
        }
    });

    proc->bind({
        "cart rm",
        "Remove a game from your cart",
        [this](auto args)
        {
            if (args.size() < 1)
                *cout << "Please specify the name of the game you want to remove\n";
            else
                removeFromCart(args[0]);
        }
    });

    proc->bind({
        "cart buy",
        "Buy all the games in your cart",
        [this](auto args)
        {
            buyGamesInCart();
        }
    });

    proc->bind({
        "cart show",
        "Show games currently in your cart",
        [this](auto args)
        {
            logCart();
        }
    });

    proc->bind({
        "login",
        "Log in with the supplied username and password",
        [this](auto args) 
        {
            if (args.size() != 2)
                *cout << "please provide a username and password";
            else
                login(args[0], args[1]);
        }
    });

    proc->bind({
        "logout",
        "Log out the current user",
        [this](auto args) 
        {
            logout(); 
        }
    });

    proc->bind({
        "help",
        "Show command line information",
        [this](auto args)
        {
            logHelp(); 
        }
    });
}
