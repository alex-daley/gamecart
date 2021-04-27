#include "order_service.h"

OrderService::OrderService(Database* database) : database(database)
{
}

void OrderService::insert(const Order& order)
{
    static constexpr auto INSERT_ORDER = R"(
    INSERT INTO
        Orders (game_id, user_id, price) 
    VALUES
        (?, ?, ?)
    )";

    database->prepare(INSERT_ORDER)
        .bindInteger(order.gameID)
        .bindInteger(order.userID)
        .bindDouble(order.cost)
        .execute();
}

std::vector<OrderMetrics> OrderService::getOrderMetrics() const
{

    static constexpr auto GET_METRICS = R"(
        SELECT name, SUM(Orders.price) as revenue, COUNT(Orders.uid)
        FROM Orders
        INNER JOIN Games 
        ON Orders.game_id = Games.uid
        GROUP BY name
        ORDER BY revenue DESC
    )";

    auto row = database->prepare(GET_METRICS)
        .execute();

    std::vector<OrderMetrics> metrics;
    do
    {
        OrderMetrics entry{};
        entry.gameName = row.getText(0);
        entry.revenue = row.getDouble(1);
        entry.sales = row.getInteger(2);
        metrics.push_back(entry);
    }
    while (row.step());

    return metrics;
}
