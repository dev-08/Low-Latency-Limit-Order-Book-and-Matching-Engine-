#pragma once 
#include "Order.h"
#include <map>
#include <deque>
#include <functional>
#include <unordered_map>

class OrderBook {
    private :

        std::map<double, std::deque<Order>, std::greater<double>> bids;
        std::map<double,std::deque<Order>> asks;

        std::vector<Trade> trades;
        std::unordered_map<int,Order> allOrders;

        long long getCurrentTimestamp() const;
        int nexorderId = 1;


    public :
        int addOrder(Side side,double price, int quantity);
        void cancleOrder(int orderId);
        void printBook() const;
        void printBestBidAsk() const;
        void printTrades() const;
        void printAllOrders() const;

    private:
        void matchBuyOrder(Order& incoming);
        void matchSellOrder(Order& incoming);
        void updateOrderStatus(Order& order);

};