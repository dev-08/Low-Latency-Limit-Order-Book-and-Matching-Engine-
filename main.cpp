#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<limits>
#include "OrderBook.h"
#include"Benchmark.h"






 void replayFromCSV(OrderBook& book, const std::string& filename)
    {
    std::ifstream file(filename);

    if(!file.is_open()){
        std::cout<<"Failed to Open File" << filename << "\n" ;
        return ;
    }

    std::string line;

    std::getline(file,line);

    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string action, sideStr, priceStr, qtyStr,orderIdstr;

        std::getline(ss,action,',');
        std::getline(ss,sideStr, ',');
        std::getline(ss,priceStr,',');
        std::getline(ss,qtyStr,',');
        std::getline(ss,orderIdstr,',');


    if(action == "ADD"){
        double price = std::stod(priceStr);
        int qty = std::stoi(qtyStr);

        Side side = (sideStr == "BUY") ? Side::Buy  : Side::Sell;

        int newOrderId = book.addOrder(side,price,qty);

        std::cout<<"Replayed Add -> new Order ID: " << newOrderId
        <<"Side = "<< sideStr
        <<" price = " <<price 
        <<"Qty = "<<qty << " \n ";
    }else if( action == "CANCEL"){
        int orderId = std::stoi(orderIdstr);
        book.cancleOrder(orderId);

        std::cout<< "Replayed CANCEL -> Order ID: "<< orderId << "\n";
    }


    }
    file.close();
    }

int main(){
  
    OrderBook book;
    int choice;
    while (true)
    {
        std:: cout <<"\n1. Add Buy Order \n";
        std:: cout<<"2. Add Sell Order \n";
        std:: cout<<"3. Cancel Order \n";
        std:: cout<<"4. Print Book \n";
        std:: cout<<"5. Print Best Bid/Ask \n";
    
        std::cout<<"6. Print Trades \n";
        std:: cout<<"7. Print All Orders \n";
         std:: cout<<"8. Import From CSV \n";
                std:: cout<<"9. Run Benchmark \n";
       std:: cout<<"10. Exit \n";
        std:: cout<< "Enter Choice ";
        std:: cin>> choice;

        if(choice == 1 || choice == 2){
            double price;
            int qty; 
            std::cout<< "Enter Price : ";
            if(!(std::cin>> price))
            {
                std::cout<< "Invalid Price.\n";
                std::cin.clear();
                std::cin.ignore(10000,'\n');
                continue;
            }

            std::cout<<"Enter Quantity";

            if(!(std::cin>>qty)){
                std::cout<< "Invalid Quantity. Enter a whole number.\n";
                std::cin.clear();
                std::cin.ignore(10000,'\n');
                continue;
            }
            int orderId = book.addOrder(choice == 1 ? Side:: Buy : Side::Sell, price,qty);
            std::cout<<"Order Added. ID = "<< orderId << "\n";
        }else if( choice == 3){
            int orederId; 
            std::cout<< "Enter Order ID to Cancel: ";

            if(!(std::cin>>orederId)){
                std::cout<< "Invalid Order .\n";
                std::cin.clear();
                std::cin.ignore(10000,'\n');
                continue;
            }
             book.cancleOrder(orederId);
        }else if(choice == 4){
            book.printBook();
        }else if(choice == 5){
            book.printBestBidAsk();
        }else if(choice == 6){
            book.printTrades();
        }else if(choice == 7){
            book.printAllOrders();
        }else if(choice == 8){
            std::string filename;
            std::cout<< "Enter CSV File Name : ";
            std::cin>> filename;
            replayFromCSV(book,filename);
        }else if(choice == 9){
            runBenchmark();
        }
        else if(choice == 10){
            break;
        }else{
            std:: cout<< "invalid choice. \n";
        }
        
    }
    return 0;

}

