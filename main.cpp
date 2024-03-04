#include<pcap/pcap.h>
#include<bits/stdc++.h>
#include "udp.h"
#include "cbe.h"
#include "snapshotRefresh.h"
#include "orderbook.h"
#include "utils.h"
#include "snapshotRefreshBook.hpp"

using namespace std;
bool DEBUG = false;

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "Pass a pcap file as argument.\n";
        return 1;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(argv[1], errbuf);

    if (handle == nullptr) {
        std::cerr << "pcap_open_offline() failed: " << errbuf << std::endl;
        return 1;
    }

    const u_char* packet;
    struct pcap_pkthdr header;

    int pckt_num = 0;
    OrderBookCollection orderbooks;

    while ((packet = pcap_next(handle, &header)) != nullptr) {
        pckt_num++;
        UDP udp = UDP(packet + 34);
        if (udp.src_port == 319 || udp.src_port == 320) continue;

        CBE cbe = CBE(udp.payload, udp.payload_length);
        for (Message msg:cbe.messages){
            if(msg.template_id == 52){
                SnapShotFullRefresh snapshot(msg.payload);
                orderbooks.updateOrderBook(snapshot, cbe.header.sendtime);
            }
        }
    }
    pcap_close(handle);

    std::cout << "ORDER BOOK\n";
    while(1){
        std::cout << "--------------------------------\n";
        std::cout << "1. LIST SECURITY IDs AVAILABLE\n2. PRINT ORDER BOOK\n3. EXIT\nCHOOSE OPTION NUMBER: ";
        int command;
        std::cin >> command;
        if(cin.fail()){
            std::cout << "Invalid Input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Remove bad input
        }
        else if(command == 1){
            std::cout << std::endl;
            orderbooks.printSecurityIDs();
        }
        else if(command == 2){
            std::cout << "\nENTER SECURITY ID(0 for random one): ";
            int sec_id;
            std::cin >> sec_id;
            std::cout << '\n';
            orderbooks.printOrderBook(sec_id);
        }
        else if(command == 3){
            break;
        }
        else{
            std::cout << "Invalid Input.\n";
        }
        std::cout << std::endl;
    }

    return 0;
}