#pragma once
#include <istream>
#include <ostream>
#include <string>
#include <optional>
#include <charconv>
#include <vector>

class Console
{
public:
    Console(std::istream* in, std::ostream* out);
    Console();

    template <typename T> 
    std::ostream& operator << (T out);

    template <typename T> 
    std::istream& operator >> (T& in);

    std::string read_line() const;

    template <typename T> 
    std::optional<T> read_number() const;

    int menu(std::vector<std::string> options) const;

    template <typename... Args>
    int menu(Args... args);

private:
    std::istream* m_in;
    std::ostream* m_out;
};

template <typename T>
inline std::ostream& Console::operator << (T out)
{
    *m_out << out;
    return *m_out;
}

template <typename T>
inline std::istream& Console::operator >> (T& in)
{
    *m_in >> in;
    return *m_in;
}

template <typename T>
inline std::optional<T> Console::read_number() const
{
    auto input = read_line();

    auto number = T();
    auto [ptr, err] = std::from_chars(input.data(), input.data() + input.size(), number);
    return err == std::errc() && *ptr == '\0'
        ? std::optional<T>(number)
        : std::nullopt;
}

template<typename ...Args>
inline int Console::menu(Args ...args)
{
    return menu({ args... });
}
