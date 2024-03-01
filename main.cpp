#include<pcap/pcap.h>
#include<bits/stdc++.h>
#include "udp.h"
#include "cbe.h"
#include "snapshotRefresh.h"
#include "orderbook.h"
#include "utils.h"

using namespace std;
bool DEBUG = false;


void process_snapshot_full_refresh(const u_char* packet){

}

void process_messages(const u_char* packet){
    int64_t sending_time = convertBinaryToInt64(packet, 4, 8);
    
}

int main(int argc, char* argv[]){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(argv[1], errbuf);

    if (handle == nullptr) {
        std::cerr << "pcap_open_offline() failed: " << errbuf << std::endl;
        return 1;
    }

    const u_char* packet;
    struct pcap_pkthdr header;
    set<int32_t> ports;
    set<int32_t> template_ids;


    int pckt_num = 1;
    OrderBook orderbook(16777);
    while ((packet = pcap_next(handle, &header)) != nullptr) {
        UDP udp = UDP(packet + 34);
        if (udp.src_port == 319 || udp.src_port == 320) continue;

        CBE cbe = CBE(udp.payload, udp.payload_length);
        // std::cout << pckt_num << std::endl;
        for (Message msg:cbe.messages){
            if(msg.template_id == 52){
                SnapShotFullRefresh snapshot(msg.payload);
                // std::cout << snapshot.security_id << endl;
                for (int i = 0; i < snapshot.ssrgs.group_cnt; i++){
                    orderbook.addEntry(snapshot.ssrgs.groups[i], cbe.header.sendtime);
                }
                // break;
                // return 0;
                // if(DEBUG){
                //     cout << pckt_num << " Sendtime: " <<  epochToReadable(cbe.header.sendtime) << "\t Sequence Number: " << cbe.header.msg_seq_num << endl;
                //     cout << "Last MSG seq: " << snapshot.lastmsgSeqNumProcessed << "\t RptSeq: " << snapshot.rpt_seq << endl;
                //     cout << "Transact Time: " << epochToReadable(snapshot.transact_time) << endl;
                //     if (pckt_num > 10000) {
                //         return 0;
                //     }
                // }
            }
        }
        pckt_num++;
        // ports.insert(udp_src_port);
        // template_ids.insert(template_id);
    }
    orderbook.printBook();

    pcap_close(handle);
    // for (auto port: ports){
    //     std::cout << port << endl;
    // }
    // for (auto templ : template_ids){
    //     std::cout << templ << std::endl;
    // }
    return 0;
}