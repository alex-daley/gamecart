#include "user.hpp"
#include "utils.hpp"
#include <chrono>

User::User(
    int id,
    const std::string& username, 
    const std::string& dateOfBirth, 
    const std::string& email) :

    id(id),
    username(username),
    dateOfBirth(dateOfBirth),
    email(email)
{
}

int User::getID() const
{
    return id;
}

const std::string& User::getUsername() const
{
    return username;
}

void User::setUsername(const std::string& username)
{
    this->username = username;
}

const std::string& User::getDateOfBirth() const
{
    return dateOfBirth;
}

void User::setDateOfBirth(const std::string& dateOfBirth)
{
    this->dateOfBirth = dateOfBirth;
}

const std::string& User::getEmail() const
{
    return email;
}

void User::setEmail(const std::string& email)
{
    this->email = email;
}

int User::getAge() const
{
    using Clock = std::chrono::system_clock;

    std::time_t start = Utils::dateParse(dateOfBirth);
    std::time_t now = Clock::to_time_t(Clock::now());
    
    // Get seconds between dates.
    double seconds = std::difftime(now, start);

    // Convert seconds to years.
    constexpr int secondsInYear = 31536000;
    double years = seconds / secondsInYear;

    return (int)std::floor(years);
}
