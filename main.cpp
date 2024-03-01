#include<pcap/pcap.h>
#include<iostream>
#include<map>
#include<vector>
#include<set>
#include<bits/stdc++.h>

using namespace std;

int32_t convertBinaryToInt(const u_char* data, int offset, int length) {
    int32_t result = 0;
    for (int i = 0; i < length; ++i) {
        result <<= 8;
        result |= (data[offset + i] & 0xFF);
    }
    return result;
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

    int i = 0;
    while ((packet = pcap_next(handle, &header)) != nullptr) {
        int32_t udp_src_port = (int32_t)convertBinaryToInt(packet, 34, 2);
        i++;
        ports.insert(udp_src_port);


        // std::cout << "Packet capture length: " << header.caplen << std::endl;
        // std::cout << "Packet total length: " << header.len << std::endl;

        // // Convert timestamp to readable format
        // std::time_t packet_time = header.ts.tv_sec;
        // std::string time_str = std::ctime(&packet_time);
        // std::cout << "Timestamp: " << time_str.substr(0, time_str.length() - 1) << std::endl;  // Remove trailing newline

        // std::cout << "------" << std::endl;
        // Assuming packet is a pointer to the start of the Ethernet frame
        // template_ids.insert(packet[58]);
    }

    pcap_close(handle);
    for (auto port: ports){
        std::cout << port << endl;
    }
    return 0;
}