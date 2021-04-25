#pragma once
#include <string>
#include <vector>

struct Game
{
    int uid;
    std::string name;
    std::string genre;
    int ageRating;
    double price;
    int copies;

    std::vector<std::string> toStringArray() const
    {
        return
        {
            name,
            genre,
            std::to_string(ageRating),
            std::to_string(price),
            std::to_string(copies)
        };
    }
};

// This comparison functions allows us store Game objects in a std::set (keyed by uid).
inline 
bool operator < (const Game& lhs, const Game& rhs)
{
    return lhs.uid < rhs.uid;
}
