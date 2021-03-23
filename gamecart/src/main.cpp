#include <charconv>
#include <optional>
#include <iostream>
#include <string>

namespace {

    struct User {
        std::string name;
        std::string email;
        int age;
    };

    template <typename T>
    std::optional<T> parse_number(std::string input) {
        T result = 0;
        auto [ptr, error] = std::from_chars(input.data(), input.data() + input.size(), result);
        return error == std::errc() && *ptr == '\0'
            ? std::optional<T>(result)
            : std::nullopt;
    }    

    std::string read_line() {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    User request_user_details() {
        auto user = User();
        std::printf("please input your user details\n");
        std::printf("Name\n");
        user.name = read_line();

        std::printf("Email\n");
        user.email = read_line();

        while (true) {
            std::printf("Age\n");
            auto age = parse_number<int>(read_line());
            if (age) {
                user.age = *age;
                break;
            }
        }

        return user;
    }
}

int main() {
    User user = request_user_details();
    return 0;
}
