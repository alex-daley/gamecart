#include <memory>
#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "application.hpp"
#include "command.hpp"
#include "command_processor.hpp"

int main() {
    auto processor = std::make_unique<CommandProcessor>();
    auto app = Application(processor.get());
    app.run();

    return 0;
}
