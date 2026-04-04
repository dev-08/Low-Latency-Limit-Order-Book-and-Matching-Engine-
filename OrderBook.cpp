#include "OrderBook.h"
#include <iostream>
#include <algorithm>
#include <chrono>




std:: string statusToString(OrderStatus status){
    switch (status)
    {
        case OrderStatus :: Open : return "Open";

        case OrderStatus :: PartiallyFilled : return "PartiallyFilled";

        case OrderStatus :: Filled : return "Filled";

        case OrderStatus :: Canceled : return "Canceled";

        default:return "Unknown";
    }
}


std:: string sideToString(Side side){
    return side == Side::Buy ? "Buy" : "Sell";
}
int OrderBook:: addOrder(Side side, double price, int quantity)
{
    Order incoming{ nexorderId++,  side, price,quantity, quantity,getCurrentTimestamp(),OrderStatus :: Open };
    if(side == Side::Buy){
        matchBuyOrder(incoming);

        if(incoming.quantity>0){
            bids[incoming.price].push_back(incoming);
        }
    }else {
        matchSellOrder(incoming);
        if(incoming.quantity>0){
            asks[incoming.price].push_back(incoming);
        }
    }
    updateOrderStatus(incoming);
    allOrders[incoming.orderId]  = incoming;

    return incoming.orderId;
}


void OrderBook::matchBuyOrder(Order& incoming){
    while(incoming.quantity>0 && !asks.empty()){
        auto bestAskIt = asks.begin();
        double bestAskPrice = bestAskIt->first;
        if(incoming.price<bestAskPrice){
            break;
        }

        auto& queue = bestAskIt->second;

        while(incoming.quantity > 0 && !queue.empty() ){
            Order& resting = queue.front();

            int tradedQty = std::min(incoming.quantity, resting.quantity);

            std::cout<< " TRADE: BuyOrder #" << incoming.orderId
                        << " matched SellOrder #"<<resting.orderId
                        <<" Price = "<<resting.price
                        <<" Qty = "<<tradedQty<<"\n";


                        trades.push_back({
                            incoming.orderId,
                            resting.orderId,
                            resting.price,
                            tradedQty,
                            getCurrentTimestamp(),
                        });

                        incoming.quantity = incoming.quantity - tradedQty;
                        resting.quantity = resting.quantity - tradedQty;

                        updateOrderStatus(resting);
                        if(allOrders.find(resting.orderId) != allOrders.end()){
                            allOrders[resting.orderId] = resting;
                        }

                        if(resting.quantity == 0){
                            queue.pop_front();
                        } 
        }

        if(queue.empty()){
            asks.erase(bestAskIt);
        }

    }
}


void OrderBook :: matchSellOrder(Order& incoming){
    while(incoming.quantity>0 && !bids.empty()){
        auto bestBidIt = bids.begin();
        double bestBidPrice = bestBidIt->first;

        if(incoming.price>bestBidPrice){
            break;
        }

        auto& queue = bestBidIt->second;


        while(incoming.quantity >0 && !queue.empty()){
            Order& resting  = queue.front();

            int tradedQty = std::min(incoming.quantity,resting.quantity);

            std::cout<<" TRADE : SellOrder#" << incoming.orderId
                        <<" matched BuyOrder#" << resting.orderId
                        <<" Price = "<< resting.price
                        <<" Qty = "<< tradedQty <<"\n";

                        trades.push_back({
                            resting.orderId,
                            incoming.orderId,
                            resting.price,
                            tradedQty,
                            getCurrentTimestamp()
                        });

            incoming.quantity = incoming.quantity - tradedQty;
            resting.quantity = resting.quantity - tradedQty;
            updateOrderStatus(resting);

            if(allOrders.find(resting.orderId) != allOrders.end()){
                allOrders[resting.orderId] = resting;
            }

            if(resting.quantity == 0){
                queue.pop_front();
            }

        }

        if(queue.empty()){
            bids.erase(bestBidIt);
        }
    }
}

void OrderBook::printBook() const{
    std:: cout<<"\n --- orderBook --- \n";

    std::cout<<"SELL ORDER:\n";

    for(const auto& [price,orders] : asks){
        int totalQty = 0;
        for(const auto& order: orders){
                totalQty =  totalQty + order.quantity;
        }
        std::cout<< price << " : "<< totalQty << "\n";        
    }

    std::cout<< "BUY ORDERS: \n";
    for(const auto& [price,orders]: bids){
        int totalQty = 0;
        for(const auto& order: orders){
            totalQty += order.quantity;
        }
        std:: cout<< price << " : "<< totalQty << "\n";
    }
    std::cout<< "-----------------\n";
}


void OrderBook:: printBestBidAsk() const
{
    std::cout<< "\n Best Bid: ";
    if(!bids.empty()){
        std::cout<<bids.begin()->first;
    }else{
        std::cout<<"NONE" ;
    }
    std::cout<<" | Best Ask : ";
    if(!asks.empty()){
        std::cout<<asks.begin()->first;
    }
    else{
        std::cout<<"NONE";
    }
    std::cout<<"\n";
}

void OrderBook:: printTrades() const
{
    std:: cout<< "\n ----- TRADE HISTORY -----\n";
    if(trades.empty()){
        std::cout<<"No Trades Executed. \n";
        return;
    }
    for(const auto& trade: trades){
        std::cout<< " BuyOrder #" << trade.buyOrderId
                    << " SellOrder #" << trade.sellOrderId
                    <<" price = "<< trade.price
                    <<" Qty = "<< trade.quantity
                    <<" timestemp ="<<trade.timestemp << "\n";
    }
}


void OrderBook:: cancleOrder(int orderID){
    for(auto it = bids.begin(); it != bids.end(); ++it){
        auto& queue = it->second;
        for(auto qit = queue.begin(); qit!= queue.end(); ++qit){
            if(qit->orderId == orderID){
                
                qit->quantity = 0;
                qit->status = OrderStatus::Canceled;
                allOrders[orderID] = *qit;

                queue.erase(qit);
                if(queue.empty()){
                    bids.erase(it);
                }

                std:: cout<< "Canceld But Order #" << orderID << "\n";
                return;
            }
        }
    }

    for(auto it = asks.begin(); it != asks.end(); ++it){
        auto& queue = it->second;
        for(auto qit = queue.begin(); qit != queue.end(); ++qit){
            if(qit->orderId == orderID){
                queue.erase(qit);
                if(queue.empty()){
                    asks.erase(it);
                }

                std:: cout<<"Canceld Sell Order #" << orderID << "\n";
                return;
            }
        }
    }

    std::cout <<"Order #" << orderID << "not found.\n";
}



void OrderBook :: updateOrderStatus(Order& order){
    if(order.status == OrderStatus::Canceled)
        return;

    if(order.quantity == order.originalQuantity)
        order.status = OrderStatus::Open;
    else if(order.quantity == 0)
        order.status = OrderStatus::Filled;
    else    
        order.status = OrderStatus::PartiallyFilled;
}


void OrderBook::printAllOrders() const{
    std::cout<<"\n ----- ALL ORDERS -----\n";

    if(allOrders.empty()){
        std::cout<<"NO Orders Avaialble. \n";
        return;
    }

    for(const auto& pair : allOrders ){
        const Order& order = pair.second;

        std::cout<< " ORDER ID = "<< order.orderId
                <<" Side = " << sideToString(order.side)
                <<" Price = "<<order.price
                <<" RemainingQty = "<< order.quantity
                <<" originalQty = "<<order.originalQuantity
                <<" Status = "<< statusToString(order.status)
                <<" TimeStamp = "<< order.timestamp
                <<"\n" ;
    }
}





long long OrderBook:: getCurrentTimestamp() const{
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}