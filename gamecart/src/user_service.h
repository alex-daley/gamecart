#pragma once

#include <string>
#include "database.h"

struct User
{
    int uid;
    std::string username;
    std::string dateOfBirth;
    std::string email;
    
    // Age is calculated from dateOfBirth.
    int age() const;
};

struct LoginInfo
{
    std::string username;
    std::string insecurePassword; // Should be a hash.
};

struct CreateUserInfo : public LoginInfo
{
    std::string dateOfBirth;
    std::string email;
};

class IUserService
{
public:
    virtual ~IUserService() {}

    virtual void insert(CreateUserInfo user) = 0;
    virtual User login(LoginInfo info) = 0;
};

class UserService : public IUserService
{
public:
    UserService(Database* database);
    
    void insert(CreateUserInfo user) override;
    User login(LoginInfo info) override;

private:
    Database* database;
};
