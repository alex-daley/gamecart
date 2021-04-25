#pragma once
#include <string>
#include <vector>
#include <functional>

struct Command
{
    std::string name;
    std::string description;
    std::function<
        void([[maybe_unused]] std::vector<std::string> args)>
    callback;
};
