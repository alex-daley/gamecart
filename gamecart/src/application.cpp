#include "application.hpp"

Application::Application(Console console, Database* database) : console(console), database(database)
{
}

void Application::run()
{
    switch (console.menu("Create Account", "Login"))
    {
        case 0:
            create_account_dialog();
            [[fallthrough]];
        case 1: 
            login_dialog();
            break;
    }
}

void Application::create_account_dialog()
{
    console << "Create Account";
    console << "Username: \n";
    std::string username = console.read_line();
    if (username_exists(username))
    {
        console << "Error: username already taken\n";
        create_account_dialog();
    }

    console << "Date of Birth: \n";
    std::string date_of_birth = console.read_line();

    database->prepare("INSERT INTO User (username, date_of_birth, is_admin) VALUES (?, ?, 0)")
        .bind_text(username)
        .bind_text(date_of_birth)
        .execute();
}

void Application::login_dialog()
{
    console << "Login";
    console << "Please enter your username:\n";
    auto username = console.read_line();
    if (!username_exists(username))
    {
        login_dialog();
    }
}

bool Application::username_exists(std::string username) const
{
    return database->prepare("SELECT 1 FROM User WHERE username = ?")
        .bind_text(username)
        .execute();
}
