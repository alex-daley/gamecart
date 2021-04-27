#pragma once

#include <string>
#include <vector>
#include "database.h"

struct Order
{
    int uid;
    int gameID;
    int userID;
    double cost;
};

struct OrderMetrics
{
    std::string gameName;
    double revenue;
    int sales;
};

class IOrderService
{
public:
    virtual ~IOrderService() {}
    virtual void insert(const Order& order) = 0;
    virtual std::vector<OrderMetrics> getOrderMetrics() const = 0;
};

class OrderService : public IOrderService
{
public:
    OrderService(Database* database);
    void insert(const Order& user) override;
    std::vector<OrderMetrics> getOrderMetrics() const override;

private:
    Database* database;
};
