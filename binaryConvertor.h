#ifndef Binary_Convertor_Header
#define Binary_Convertor_Header

#include<bits/stdc++.h>

// SBE uses Little Endian, Network Protocols use Big Endian Format
// max support 8bytes
int64_t convertBinaryToInt64(const u_char* data, int offset, int length, bool littleEndian = false) {
    int64_t result = 0;
    if (littleEndian == false){
        for(int i = 0; i < length; ++i)
        {
            result <<= 8;
            result |= (data[offset + i] & 0xFF);
        }
    } 
    else {
        for(int i = length - 1; i >= 0; --i)
        {
            result <<= 8;
            result |= (data[offset + i] & 0xFF);
        }
    }
    return result;
}

#endif