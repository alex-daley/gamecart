#pragma once
#include <map>

class Cart
{
public:
    using GameID = int;

    int& operator [](GameID id);

    const std::map<GameID, int>& getOrders() const;
    int getCount(GameID uid) const;
    bool getIsEmpty() const;

    void addOrder(GameID id);
    void addOrder(GameID id, int copies);

    void removeOrder(GameID id);
    void removeOrder(GameID id, int copies);

    // Remove order reduces the number of copies of a order.
    // eraseAt removes the entire entry from the order map.
    void eraseAt(GameID id);
    void clear();

private:
    // Maps game ID to copies requested.
    std::map<GameID, int> orderCount;
};
