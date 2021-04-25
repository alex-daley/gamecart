#pragma once
#include <optional>
#include <string>
#include "user.hpp"
#include "database.hpp"

struct LoginRequest
{
    std::string username;
    std::string insecurePassword;
};

struct CreateAccountRequest
{
    std::string username;
    std::string insecurePassword;
    std::string dateOfBirth;
    std::string email;
};

class UserManager
{
public:
    UserManager(Database& database);
    
    bool isLoggedIn() const;

    void createAccount(const CreateAccountRequest& request);
    void login(const LoginRequest& request);
    void logout();

private:
    Database& database;
    std::optional<User> user;
};
