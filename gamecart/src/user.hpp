#pragma once
#include <string>

class User
{
public:
    User(
        int id,
        const std::string& username, 
        const std::string& dateOfBirth,
        const std::string& email);

    User() = default;

    int getID() const;

    const std::string& getUsername() const;
    void setUsername(const std::string& username);
    
    const std::string& getDateOfBirth() const;
    void setDateOfBirth(const std::string& dateOfBirth);

    const std::string& getEmail() const;
    void setEmail(const std::string& email);

    int getAge() const;

private:
    int id;
    std::string username;
    std::string dateOfBirth;
    std::string email;
};
