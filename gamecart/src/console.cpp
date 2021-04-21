#include <iostream>
#include "console.hpp"

Console::Console(std::istream* in, std::ostream* out) : m_in(in), m_out(out)
{ 
}

Console::Console() : Console(&std::cin, &std::cout)
{
}

std::string Console::read_line() const
{
    std::string line;
    std::getline(*m_in, line);
    return line;
}

int Console::menu(std::vector<std::string> options) const
{
    auto i = 1;
    for (auto& option : options)
        std::cout << i++ << ": " << option << std::endl;

    auto choice = read_number<int>();
    return choice && *choice > 0 && *choice <= (int)options.size()
        ? *choice - 1
        : menu(options);
}
