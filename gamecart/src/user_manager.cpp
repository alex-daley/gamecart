#include <stdexcept>
#include "user_manager.hpp"

UserManager::UserManager(Database& database) : database(database)
{
    
}

bool UserManager::isLoggedIn() const
{
    return user.has_value();
}

void UserManager::createAccount(const CreateAccountRequest& request)
{
    auto row = database
        .prepare("SELECT 1 FROM Users WHERE username = ?")
        .bindText(request.username)
        .execute();

    if (row)
    {
        throw std::runtime_error("Username already taken");
    }
    
    if (request.insecurePassword.length() < 5)
    {
        throw std::runtime_error("Password must be greater than 5 characters long");
    }

    database
        .prepare("INSERT INTO Users (username, insecure_password, date_of_birth, email) VALUES (?, ?, ?, ?)")
        .bindText(request.username)
        .bindText(request.insecurePassword) // THIS SHOULD BE HASHED!
        .bindText(request.dateOfBirth)
        .bindText(request.email)
        .execute();
}

void UserManager::login(const LoginRequest& loginInfo)
{
    auto row = database
        .prepare("SELECT id, insecure_password, date_of_birth, email FROM Users WHERE username = ?")
        .bindText(loginInfo.username)
        .execute();

    if (!row || row.getText(1) != loginInfo.insecurePassword)
    {
        throw std::runtime_error("Invalid username or password");
    }
    else
    {
        User user(
            row.getInteger(0),   // id
            loginInfo.username,  // username
            row.getText(2),      // date of birth
            row.getText(3)       // email
        );

        this->user = user;
    }
}

void UserManager::logout()
{
    user = std::nullopt;
}
