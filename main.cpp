#include<pcap/pcap.h>
#include<bits/stdc++.h>
#include "udp.h"
#include "cbe.h"
#include "snapshotRefresh.h"
#include "orderbook.h"
#include "utils.h"

using namespace std;
bool DEBUG = false;

int main(int argc, char* argv[]){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(argv[1], errbuf);

    if (handle == nullptr) {
        std::cerr << "pcap_open_offline() failed: " << errbuf << std::endl;
        return 1;
    }

    const u_char* packet;
    struct pcap_pkthdr header;

    int pckt_num = 1;
    OrderBookCollection orderbooks;

    while ((packet = pcap_next(handle, &header)) != nullptr) {
        UDP udp = UDP(packet + 34);
        if (udp.src_port == 319 || udp.src_port == 320) continue;

        CBE cbe = CBE(udp.payload, udp.payload_length);
        // std::cout << pckt_num << std::endl;
        for (Message msg:cbe.messages){
            if(msg.template_id == 52){
                SnapShotFullRefresh snapshot(msg.payload);
                // if(snapshot.security_id != 212679) continue;
                orderbooks.updateOrderBook(snapshot, cbe.header.sendtime);
            }
        }
        pckt_num++;
    }

    orderbooks.printSecurityIDs();
    orderbooks.printOrderBook(167677);
    std::cerr << pckt_num << std::endl;




    pcap_close(handle);
    return 0;
}