#ifndef UTILS_H
#define UTILS_H

#include<bits/stdc++.h>
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
#endif