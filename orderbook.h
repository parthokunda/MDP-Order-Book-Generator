#ifndef ORDER_BOOK_HEADER
#define ORDER_BOOK_HEADER
#include<bits/stdc++.h>
#include"snapshotRefresh.h"

enum OrderEntryType {
    Bid = 0,
    Ask = 1,
};

struct OrderBookEntry{
    int64_t sending_time;
    int order_count; 
    int quantity;
    double price;
    int price_level;
    int entry_type;
    OrderBookEntry() = default;
    OrderBookEntry(SnapShotRefreshGroup ssrg, int64_t sending_time){
        this->sending_time = sending_time;
        this->order_count = ssrg.num_orders;
        this->quantity = ssrg.entry_sz;
        this->price = ssrg.entry_px;
        this->price_level = ssrg.price_level;
    }
    bool updateEntry(SnapShotRefreshGroup ssrg, int64_t sending_time){
        if (this->sending_time > sending_time){
            std::cout << "Already Updated. Skipping\n";
            return false;
        }
        this->sending_time = sending_time;
        this->order_count = ssrg.num_orders;
        this->quantity = ssrg.entry_sz;
        this->price = ssrg.entry_px;
        this->price_level = ssrg.price_level;
        return 1;
    }
};

struct OrderBook{
    int32_t security_id;
    std::map<int, OrderBookEntry>bids;
    std::map<int, OrderBookEntry>asks;
    bool addEntry(SnapShotRefreshGroup ssrg, int64_t sending_time){
        std::cout << "addEntry\n";
        int level = ssrg.price_level;
        if (ssrg.entry_type == 0){
            std::cout << "adding\n";
            if(bids.find(level) == bids.end()){
                bids[level] = OrderBookEntry(ssrg, sending_time);
                return true;
            }
            else return bids[level].updateEntry(ssrg, sending_time);
            std::cout << "done\n";
        }
        // else if(ssrg.entry_type == 1){
        //     if(asks.find(level) == asks.end()){
        //         asks[level] = OrderBookEntry(ssrg, sending_time);
        //         return true;
        //     }
        //     else return asks[level].updateEntry(ssrg, sending_time);
        // }
        else {
            printf("What type of entry type is this\n");
            std::cout << ssrg.entry_type << std::endl;
            return false;
        }
        std::cout << "addEntry done\n";
    }

    void printBook()
    {
        std::cout << std::left << std::setw(12) << "Bid"
                  << std::left << std::setw(12) << "Order Count"
                  << std::left << std::setw(12) << "Quantity"
                  << std::left << std::setw(12) << "Price"
                  << std::left << std::setw(12) << "Price"
                  << std::left << std::setw(12) << "Quantity"
                  << std::left << std::setw(12) << "Order Count"
                  << std::left << std::setw(12) << "Ask" << std::endl;

        // Assume both maps have the same size, otherwise you need a more complex logic
        auto bidIt = bids.begin();
        auto askIt = asks.begin();

        while (bidIt != bids.end() || askIt != asks.end())
        {
            if (bidIt != bids.end())
            {
                std::cout << std::left << std::setw(12) << bidIt->second.order_count
                          << std::left << std::setw(12) << bidIt->second.quantity
                          << std::left << std::setw(12) << std::fixed << std::setprecision(2) << bidIt->second.price;
                ++bidIt;
            }
            else
            {
                std::cout << std::left << std::setw(12) << ""
                          << std::left << std::setw(12) << ""
                          << std::left << std::setw(12) << "";
            }

            if (askIt != asks.end())
            {
                std::cout << std::left << std::setw(12) << std::fixed << std::setprecision(2) << askIt->second.price
                          << std::left << std::setw(12) << askIt->second.quantity
                          << std::left << std::setw(12) << askIt->second.order_count;
                ++askIt;
            }
            else
            {
                std::cout << std::left << std::setw(12) << ""
                          << std::left << std::setw(12) << ""
                          << std::left << std::setw(12) << "";
            }
            std::cout << std::endl;
        }
    }
};

// struct OrderBookCollection{
//     std::map<int32_t, OrderBook>collection;
    
// };

#endif