#pragma once
#include <vector>
#include <string>

class Table {
public:
    void set_headings(std::vector<std::string> headings);
    void add_row(std::vector<std::string> row);
    std::string str(int column_width) const;

private:
    std::vector<std::string> headings;
    std::vector<std::vector<std::string>> rows;
};
