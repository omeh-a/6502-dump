#include <iostream>
#include <string>

#include <stdio.h>
#include "decode.h";

constexpr char ASCII_A_OFFSET = 55;
constexpr char ASCII_ZERO_OFFSET = 48;

// Given a 6502 instruction as binary, return an information string
std::string decode(std::byte instr) {
    std::string s = hexString(instr) + " ## ";

    switch(static_cast<int>(instr)) {
        case 0x00:
            s+="BRK | Break";
        case 0x01:
            s+="ORA | Break";
        
        default:
            s+=" no definition";

    }
}

// Returns 
std::string hexString(std::byte b) {
    std::string out = "0x";
    char c1 = charFormat(static_cast<char>(b) % 64);
    char c2 = charFormat(static_cast<char>(b) >> 4);

    return out + c1 + c2;
}

// Given a hex digit, return it as a char. e.g. 0xC in, 'C' out.
char charFormat(char c) {
    // check for bad input
    if (c > 16) {
        return '!';
    }

    if (c >= 10) {
        return c + ASCII_A_OFFSET;
    }
    return c + ASCII_ZERO_OFFSET;
}