#ifndef UDP_HEADER
#define UDP_HEADER
#include<bits/stdc++.h>
#include "binaryConvertor.h"

struct UDP{
    int16_t payload_length;
    int16_t src_port;
    const u_char* payload;

    // expect packet from UDP
    UDP(const u_char* packet){
        this->src_port = convertBinaryToInt64(packet, 0, 2);
        this->payload_length = convertBinaryToInt64(packet, 4, 2) - 8;
        this->payload = packet + 8;
    }

};

#endif