#pragma once
#include <vector>
#include <string>

namespace Utils
{
    std::vector<std::string> split(
        const std::string& text,
        const std::string& delimiter);

    std::string formatTable(
        const std::vector<std::string>& headings,
        const std::vector<std::vector<std::string>>& rows,
        int width = 20);

    std::string colour(
        const std::string& text,
        const std::string& asniEscapeCode = "\u001b[33m");
}
