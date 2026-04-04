#pragma once
#include <string>


enum class Side{
    Buy,
    Sell
};

enum class OrderStatus{
    Open,
    PartiallyFilled,
    Filled,
    Canceled
};

struct  Trade
{
    int buyOrderId;
    int sellOrderId;
    double price;
    int quantity;
    long long timestemp;
};


struct Order
{
    /* data */
    int orderId;
    Side side;
    double price;
    int quantity;
    int originalQuantity;
    long long timestamp;
    OrderStatus status;

};
