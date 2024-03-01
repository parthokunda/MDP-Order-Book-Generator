#ifndef CBE_HEADER
#define CBE_HEADER

#include"binaryConvertor.h"
#include<bits/stdc++.h>

struct BinaryPacketHeader{
    int32_t msg_seq_num;
    int64_t sendtime;
    BinaryPacketHeader(const u_char* packet){
        msg_seq_num = convertBinaryToInt64(packet, 0, 4, true);
        sendtime = convertBinaryToInt64(packet, 4, 8, true);
    }
};

struct Message{
    int32_t size;
    int32_t template_id;
    const u_char* payload;
    int32_t payload_len;
    Message(const u_char* packet){
        this->size = convertBinaryToInt64(packet, 0, 2, true);
        this->template_id = convertBinaryToInt64(packet, 4, 2, true);
        this->payload = packet + 10;
        this->payload_len = size - 10;
    }
};

struct CBE {
    BinaryPacketHeader header;
    std::vector<Message> messages;
    CBE(const u_char* packet, int32_t payload_len) : header(packet){
        int32_t bytes_processed = 12;
        while(bytes_processed < payload_len){
            Message msg = Message(packet + bytes_processed);
            messages.push_back(msg);
            bytes_processed += msg.size;
        }
    }
};

#endif


