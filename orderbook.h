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
            // std::cout << "Already Updated. Skipping\n";
            return false;
        }
        this->sending_time = sending_time;
        this->order_count = ssrg.num_orders;
        this->quantity = ssrg.entry_sz;
        this->price = (double)ssrg.entry_px;
        this->price_level = ssrg.price_level;
        return 1;
    }
};

struct OrderBook{
    int32_t security_id;
    std::map<int, OrderBookEntry>bids;
    std::map<int, OrderBookEntry>asks;

    OrderBook() = default;
    OrderBook(int32_t sec_id){
        this->security_id = sec_id;
    }

    bool addEntry(SnapShotRefreshGroup ssrg, int64_t sending_time){
        int level = ssrg.price_level;
        if (ssrg.price_level == 127 && ssrg.entry_type != 6){
            std::cout << ssrg.price_level << std::endl;
            exit(0);
        }
        if (ssrg.entry_type == 0){
            // std::cout << "adding\n";
            if(bids.find(level) == bids.end()){
                bids[level] = OrderBookEntry(ssrg, sending_time);
                return true;
            }
            else return bids[level].updateEntry(ssrg, sending_time);
        }
        else if(ssrg.entry_type == 1){
            if(asks.find(level) == asks.end()){
                asks[level] = OrderBookEntry(ssrg, sending_time);
                return true;
            }
            else return asks[level].updateEntry(ssrg, sending_time);
        }
        else if(ssrg.entry_type == 6){
            return false; //settlement not important?
        }
        else {
            printf("What type of entry type is this\n");
            std::cout << ssrg.entry_type << std::endl;
            return false;
        }
    }

    void printBook()
    {
        std::cout << std::left << std::setw(12) << "Bid"
                  << std::left << std::setw(12) << "Order Count"
                  << std::left << std::setw(12) << "Quantity"
                  << std::left << std::setw(12) << "Price" << std::endl;

        for(auto bidIt = bids.begin(); bidIt != bids.end(); bidIt++)
        {
            std::cout << std::left << std::setw(12) << bidIt->second.price_level
                        << std::left << std::setw(12) << bidIt->second.order_count
                        << std::left << std::setw(12) << bidIt->second.quantity
                        << std::left << std::setw(20) << std::fixed << std::setprecision(2) << bidIt->second.price * 1e-9;
                // std::cout << std::left << std::setw(12) << ""
                //           << std::left << std::setw(12) << ""
                //           << std::left << std::setw(12) << ""
                //           << std::left << std::setw(20) << "";

            // if (askIt != asks.end())
            // {
            //     std::cout << std::left << std::setw(12) << askIt->second.price_level
            //               << std::left << std::setw(12) << askIt->second.order_count
            //               << std::left << std::setw(12) << askIt->second.quantity
            //               << std::left << std::setw(12) << std::fixed << std::setprecision(2) << askIt->second.price;
            //     ++askIt;
            // }
            // else
            // {
            //     std::cout << std::left << std::setw(12) << ""
            //               << std::left << std::setw(12) << ""
            //               << std::left << std::setw(12) << ""
            //               << std::left << std::setw(12) << "";
            // }
            std::cout << std::endl;
        }
        std::cout << "Ask Size: " << asks.size() << std::endl;
        std::cout << std::left << std::setw(12) << "Ask"
                << std::left << std::setw(12) << "Quantity"
                << std::left << std::setw(12) << "Order Count"
                << std::left << std::setw(12) << "Price" << std::endl;
        for(auto askIt = asks.begin(); askIt != asks.end(); askIt++){
            std::cout << std::left << std::setw(12) << askIt->second.price_level
                    << std::left << std::setw(12) << askIt->second.order_count
                    << std::left << std::setw(12) << askIt->second.quantity
                    << std::left << std::setw(20) << std::fixed << std::setprecision(2) << askIt->second.price * 1e-9
                    << std::endl;
        }
        std::cout << std::endl;
    }
};

struct OrderBookCollection{
    std::map<int32_t, OrderBook>collection;
    void updateOrderBook(SnapShotFullRefresh snapshot, int64_t send_time){
        for (int i = 0; i < snapshot.ssrgs.group_cnt; i++){
            SnapShotRefreshGroup ssrg = snapshot.ssrgs.groups[i];
            int sec_id = snapshot.security_id;
            if(collection.find(sec_id) == collection.end()){
                collection[sec_id] = OrderBook(sec_id);
            }
            collection[sec_id].addEntry(ssrg, send_time);
        }
    }

    void printOrderBook(int32_t security_id){
        auto orderBookIt = collection.find(security_id);
        if (orderBookIt == collection.end()){
            std::cout << "Sorry. Could not find orderbook with this Security ID" << std::endl;
            return;
        }
        orderBookIt->second.printBook();
    }
    
    void printSecurityIDs(){
        std::cout << "Total OrderBooks : " << collection.size() << std::endl;
        for(auto it:collection){
            std::cout << it.first << " ";
        }
        std::cout << std::endl;
    }
};

#endif