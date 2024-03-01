#ifndef SNAPSHOT_REFRESH_HEADER
#define SNAPSHOT_REFRESH_HEADER

#include<bits/stdc++.h>
#include"binaryConvertor.h"
#include "utils.h"

struct SnapShotRefreshGroup{
    int64_t entry_px;
    int32_t entry_sz;
    int32_t num_orders;
    int16_t price_level;
    int16_t entry_type;
    SnapShotRefreshGroup(const u_char* packet){
        this->entry_px = convertBinaryToInt64(packet, 0, 8, true);
        this->entry_sz = convertBinaryToInt64(packet, 8, 4, true);
        this->num_orders = convertBinaryToInt64(packet, 12, 4, true);
        this->price_level = convertBinaryToInt64(packet, 16, 1, true);
        this->entry_type = convertBinaryToInt64(packet, 21, 1, true) - 48; // '0x30' is seen as 0.
    }
};

struct SnapShotRefreshGroups{
    //each group length
    int16_t block_len;
    int16_t group_cnt;
    std::vector<SnapShotRefreshGroup> groups;
    SnapShotRefreshGroups(){}
    SnapShotRefreshGroups(const u_char* packet){
        this->block_len = convertBinaryToInt64(packet, 0, 2, true);
        this->group_cnt = convertBinaryToInt64(packet, 2, 1, true);
        for(int i = 0; i < group_cnt; i++){
            SnapShotRefreshGroup ssrg = SnapShotRefreshGroup(packet + 3 + i * this->block_len); // 3 for the header
            groups.push_back(ssrg);
        }
    }
};

struct SnapShotFullRefresh{
    int32_t lastmsgSeqNumProcessed;
    int32_t security_id;
    int32_t rpt_seq;
    int64_t transact_time;
    int64_t update_time;
    SnapShotRefreshGroups ssrgs;
    SnapShotFullRefresh(const u_char* packet){
        this->lastmsgSeqNumProcessed = convertBinaryToInt64(packet, 0, 4, true);
        this->security_id = convertBinaryToInt64(packet, 8, 4, true);
        this->rpt_seq = convertBinaryToInt64(packet, 12, 4, true);
        this->transact_time = convertBinaryToInt64(packet, 16, 8, true);
        this->update_time = convertBinaryToInt64(packet, 24, 8, true);
        this->ssrgs = SnapShotRefreshGroups(packet + 59);
    }
    // ~SnapShotFullRefresh(){
    //     delete ssrgs;
    // }
};

#endif