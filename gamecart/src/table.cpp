#include "table.hpp"
#include <sstream>
#include <iomanip>

void Table::set_headings(std::vector<std::string> headings) {
    this->headings = headings;
}

void Table::add_row(std::vector<std::string> row) {
    rows.push_back(row);
}

std::string Table::str(int column_width) const {
    std::stringstream s;
    auto write = [&](std::string text) {
        s << std::setw(column_width) << std::left << text;
    };

    for (auto& cell : headings) {
        write(cell);
    }

    for (auto& row : rows) {
        s << std::endl;
        for (auto& cell : row) {
            write(cell);
        }
    }

    return s.str();
}
