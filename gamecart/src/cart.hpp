#pragma once
#include <string>
#include <set>

class Cart {
public:
    const std::set<int>& game_ids() const;

    void add(int game_id);
    void remove(int game_id);

private:
    std::set<int> game_ids_;
};
