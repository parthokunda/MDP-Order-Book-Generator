#ifndef SNAPSHOT_REFRESH_BOOK_HEADER
#define SNAPSHOT_REFRESH_BOOK_HEADER
#include<bits/stdc++.h>
#include "binaryConvertor.h"
#include "utils.h"

struct SnapShotRefreshBookGroup{
    int64_t order_id;
    int64_t order_priority;
    int64_t entry_px;
    int32_t display_qty;
    int16_t entry_type;
    SnapShotRefreshBookGroup(const u_char* packet){
        this->order_id = convertBinaryToInt64(packet, 0, 8, true);
        this->order_priority = convertBinaryToInt64(packet, 8, 8, true);
        this->entry_px = convertBinaryToInt64(packet, 16, 8, true);
        this->display_qty = convertBinaryToInt64(packet, 24, 4, true);
        this->entry_type = convertBinaryToInt64(packet, 28, 1, true) - 48;
    }
};

struct SnapShotRefreshBookGroups{
    int16_t block_len;
    int16_t group_cnt;
    std::vector<SnapShotRefreshBookGroup>groups;
    SnapShotRefreshBookGroups(){}
    SnapShotRefreshBookGroups(const u_char* packet){
        this->block_len = convertBinaryToInt64(packet, 0, 2, true);
        this->group_cnt = convertBinaryToInt64(packet, 2, 1, true);
        for(int i = 0; i < this->group_cnt; i++){
            SnapShotRefreshBookGroup group(packet + 3 + i * 8);
            groups.push_back(group);
        }
    }
};

struct SnapShotFullRefreshOrderBook{
    int32_t security_id;
    SnapShotRefreshBookGroups ssrbgs;
    SnapShotFullRefreshOrderBook(const u_char* packet){
        this->security_id = convertBinaryToInt64(packet, 8, 4, true);
        this->ssrbgs = SnapShotRefreshBookGroups(packet + 28);
    }
};

#endif