#include "cart.hpp"

const std::set<int>& Cart::game_ids() const {
    return game_ids_;
}

void Cart::add(int game_id) {
    game_ids_.insert(game_id);
}

void Cart::remove(int game_id) {
    game_ids_.erase(game_id);
}

