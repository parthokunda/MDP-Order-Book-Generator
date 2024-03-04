#ifndef CONSOLE_HEADER
#define CONSOLE_HEADER

#include<bits/stdc++.h>

enum COMMANDS {
    PRINT_ALL_SECURITY_ID,
    PRINT_ORDER_BOOK,
};

void printHome(){
    std::cout << "1) Print All Security ID\n2) Print Order Book\n";
}

#endif 