#include <chrono>
#include <cmath>
#include "user_service.h"
#include "utils.h"

int User::age() const
{
    using Clock = std::chrono::system_clock;

    std::time_t start = Utils::dateParse(dateOfBirth);
    std::time_t now = Clock::to_time_t(Clock::now());

    // Get seconds between date of birth and now dates...
    double seconds = std::difftime(now, start);

    // Convert seconds to years.
    constexpr int secondsInYear = 31536000;
    double years = seconds / secondsInYear;
    
    // Floor as we only care about the years.
    return (int)std::floor(years);
}

UserService::UserService(Database* database) : database(database)
{
}

void UserService::insert(CreateUserInfo user)
{
    if (user.insecurePassword.length() < 5)
    {
        throw std::runtime_error("Password must be greater than 5 characters long");
    }

    auto row = database->prepare("SELECT 1 FROM Users WHERE username = ?")
        .bindText(user.username)
        .execute();

    if (row)
    {
        throw std::runtime_error("Username already taken");
    }

    database->prepare("INSERT INTO Users (username, insecure_password, date_of_birth, email) VALUES (?, ?, ?, ?)")
        .bindText(user.username)
        .bindText(user.insecurePassword) // THIS SHOULD BE HASHED!
        .bindText(user.dateOfBirth)
        .bindText(user.email)
        .execute();
}

User UserService::login(LoginInfo info)
{
    auto row = database->prepare("SELECT id, insecure_password, date_of_birth, email FROM Users WHERE username = ?")
        .bindText(info.username)
        .execute();

    if (!row || row.getText(1) != info.insecurePassword)
    {
        throw std::runtime_error("Invalid username or password");
    }
    
    return 
    {
        row.getInteger(0), // uid
        info.username,     // username
        row.getText(2),    // date of birth
        row.getText(3)     // email
    };
}
