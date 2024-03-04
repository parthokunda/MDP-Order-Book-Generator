#ifndef UTILS_H
#define UTILS_H

#include<bits/stdc++.h>
std::string epochToReadable(int64_t epochNano) {
    auto epochSeconds = std::chrono::seconds(epochNano / 1000000000);
    auto nanos = std::chrono::nanoseconds(epochNano % 1000000000);
    auto epochTimePoint = std::chrono::time_point<std::chrono::system_clock>(epochSeconds + nanos);
    auto timeT = std::chrono::system_clock::to_time_t(epochTimePoint);

    std::tm* ptm = std::localtime(&timeT);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", ptm);

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

u_char* copyBinaryData(const u_char* source, size_t offset, size_t length) {
    u_char* data = new u_char[length];
    std::memcpy(data, source + offset, length);

    return data;
}

int getRandomNumber(int limit){
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<> distrib(0, limit);
    return distrib(generator);
}

#endif