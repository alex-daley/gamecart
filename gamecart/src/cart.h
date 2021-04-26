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

    void removeAt(GameID id);
    void clear();

private:
    // Maps game ID to copies requested.
    std::map<GameID, int> orderCount;
};
