#include "Benchmark.h"
#include "OrderBook.h"
#include <iostream>
#include <vector>
#include<limits>
#include <algorithm>
#include <chrono>

long long percentile(std:: vector<long long>& values, double p){
    if(values.empty()) return 0;
    std::sort(values.begin() , values.end());
    size_t index = static_cast<size_t>(p * (values.size() - 1));
    return values[index];
}




void runBenchmark(){
    OrderBook book;
    std::vector<long long> addLatencies;

    for(int i = 0; i<10000;i++){
        auto start = std::chrono::high_resolution_clock::now();
        book.addOrder(Side::Buy, 100.0 + (i%10), 100);
        auto end = std::chrono::high_resolution_clock::now();
        long long latency = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        addLatencies.push_back(latency);
    }

    std::cout<< "\n -------- BENCHMARK RESULTS ------ \n"; 
    std::cout<< "Add Order p50: "<< percentile(addLatencies,0.50) << "ns\n";
    std::cout<<"Add Order p95: "<<percentile(addLatencies,0.95) << "ns\n";
    std::cout<<"Add Order p99: "<<percentile(addLatencies,0.99)<<"ns\n";
}