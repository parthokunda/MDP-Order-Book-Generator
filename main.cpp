#include<pcap/pcap.h>
#include<bits/stdc++.h>
#include "udp.h"
#include "cbe.h"
#include "snapshotRefresh.h"

using namespace std;
bool DEBUG = false;

std::string epochToReadable(int64_t epochNano) {
    // Convert nanoseconds since epoch to seconds since epoch
    auto epochSeconds = std::chrono::seconds(epochNano / 1000000000);

    // Remaining nanoseconds within the current second
    auto nanos = std::chrono::nanoseconds(epochNano % 1000000000);

    // Create a system_clock time point
    auto epochTimePoint = std::chrono::time_point<std::chrono::system_clock>(epochSeconds + nanos);

    // Convert to time_t which is easier to convert to tm structure
    auto timeT = std::chrono::system_clock::to_time_t(epochTimePoint);

    // Convert to tm structure representing local time
    std::tm* ptm = std::localtime(&timeT);

    // Format the time to a human-readable string
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", ptm);

    // Add the fractional seconds part
    std::stringstream ss;
    ss << std::put_time(ptm, "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(9) << (epochNano % 1000000000);

    return ss.str();
}

void printPacket(const u_char* packet, int len){
    std::stringstream ss;
    for(int i = 0; i < len; i++){
        if (i % 16 == 0) {
            ss << std::endl;
        }
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)packet[i] << ' ';
    }
    std::cout << ss.str() << std::endl;
}

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
    while ((packet = pcap_next(handle, &header)) != nullptr) {
        UDP udp = UDP(packet + 34);
        if (udp.src_port == 319 || udp.src_port == 320) continue;

        CBE cbe = CBE(udp.payload, udp.payload_length);
        for (Message msg:cbe.messages){
            if(msg.template_id == 52){
                SnapShotFullRefresh snapshot(msg.payload);
                if(DEBUG){
                    cout << pckt_num << " Sendtime: " <<  epochToReadable(cbe.header.sendtime) << "\t Sequence Number: " << cbe.header.msg_seq_num << endl;
                    cout << "Last MSG seq: " << snapshot.lastmsgSeqNumProcessed << "\t RptSeq: " << snapshot.rpt_seq << endl;
                    cout << "Transact Time: " << epochToReadable(snapshot.transact_time) << endl;
                    if (pckt_num > 10000) {
                        return 0;
                    }
                }
            }
        }

        pckt_num++;
        // ports.insert(udp_src_port);
        // template_ids.insert(template_id);
    }

    pcap_close(handle);
    // for (auto port: ports){
    //     std::cout << port << endl;
    // }
    // for (auto templ : template_ids){
    //     std::cout << templ << std::endl;
    // }
    return 0;
}