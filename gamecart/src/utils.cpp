#include <sstream>
#include <iomanip>
#include <charconv>
#include "utils.h"

namespace
{
    template <typename T>
    std::optional<T> parseNumber(const std::string& input) 
    {
        T result = 0;
        auto [ptr, error] = std::from_chars(input.data(), input.data() + input.size(), result);
        return error == std::errc() && *ptr == '\0'
            ? std::optional<T>(result)
            : std::nullopt;
    }
}

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

    if (str != "")
    {
        fragments.push_back(str);
    }

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

    // Output table headings.
    for (const auto& cell : headings)
    {
        outputCell("[" + cell + "]");
    }

    // Output rows.
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

std::string Utils::toDecimalPlaces(double value, int places)
{
    std::stringstream s;
    s << std::fixed << std::setprecision(places) << value;
    return s.str();
}

std::optional<int> Utils::parseInt(const std::string& str)
{
    return parseNumber<int>(str);
}
