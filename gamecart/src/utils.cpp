#include <sstream>
#include <iomanip>
#include "utils.hpp"

std::vector<std::string> Utils::split(
    const std::string& text, 
    const std::string& delimiter)
{
    std::vector<std::string> fragments;
    std::string str = text; // Copy

    int pos = 0;
    while ((pos = str.find(delimiter)) != std::string::npos) 
    {
        std::string fragment = str.substr(0, pos);
        str.erase(0, pos + 1);

        if (fragment != "")
        {
            fragments.push_back(fragment);
        }
    }

    fragments.push_back(str);

    return fragments;
}

std::string Utils::formatTable(
    const std::vector<std::string>& headings, 
    const std::vector<std::vector<std::string>>& rows,
    int width)
{
    std::stringstream s;

    auto outputCell = [&](auto cell)
    {
        s << std::setw(width) << std::left << cell;
    };

    for (const auto& cell : headings)
    {
        outputCell(cell);
    }
    
    for (const auto& row : rows)
    {
        s << std::endl;
        for (const auto& cell : row)
        {
            outputCell(cell);
        }
    }

    return s.str();
}

std::string Utils::colour(
    const std::string& text, 
    const std::string& asniEscapeCode)
{
    constexpr auto white = "\u001b[37m";
    return asniEscapeCode + text + white;
}

std::time_t Utils::dateParse(const std::string& dateString)
{
    std::stringstream s(dateString);
    std::tm dateTime{};
    s >> std::get_time(&dateTime, "%d/%m/%y");
    return std::mktime(&dateTime);
}
