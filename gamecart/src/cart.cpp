#include <stdexcept>
#include <cmath>
#include "cart.h"

int& Cart::operator[](GameID id)
{
    return orderCount[id];
}

const std::map<Cart::GameID, int>& Cart::getOrders() const
{
    return orderCount;
}

int Cart::getCount(GameID uid) const
{
    try
    {
        return orderCount.at(uid);
    }
    catch (...)
    {
        return 0;
    }
}

bool Cart::getIsEmpty() const
{
    return orderCount.size() == 0;
}

void Cart::addOrder(GameID id)
{
    orderCount[id]++;
}

void Cart::addOrder(GameID id, int copies)
{
    for (int i = 0; i < copies; i++)
        addOrder(id);
}

void Cart::removeOrder(GameID id)
{
    orderCount[id] = std::max(0, orderCount[id] - 1);
}

void Cart::removeOrder(GameID id, int copies)
{
    for (int i = 0; i < copies; i++)
        removeOrder(id);
}

void Cart::eraseAt(GameID id)
{
    orderCount.erase(id);
}

void Cart::clear()
{
    orderCount.clear();
}
