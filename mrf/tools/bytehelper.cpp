#include "bytehelper.h"
#include <stdio.h>

ByteHelper::ByteHelper()
{
}


// Converts a character array containing byte information to
// a readable hexadecimal format
std::string ByteHelper::bytes2HexString( const byte_v bytes ) {
    char hexadecimal_byte[2];
    char byte;
    std::string ret;

    for ( unsigned int i = 0; i < bytes.size(); i++ ) {
        // insert spaces
        if ( i > 0 ) ret.append(" ");

        // append the byte in hexadecimal
        byte = byte2Char(bytes[i]);
        sprintf( hexadecimal_byte, "%02x", (int)byte );
        ret.append(hexadecimal_byte);
    }

    return ret;
}

// Converts a character array containing byte information to
// a readable hexadecimal format
std::string ByteHelper::bytes2BitString( const byte_v bytes ) {
    std::string singleByte;
    std::string ret;

    for ( unsigned int i = 0; i < bytes.size(); i++ ) {
        // insert spaces
        if ( i > 0 ) ret.append(" ");

        // append the byte in hexadecimal
        singleByte = bytes[i].to_string<char,std::char_traits<char>,std::allocator<char> >();
        ret.append(singleByte);
    }

    return ret;
}


// Convert a byte into a character
char ByteHelper::byte2Char( const byte B ) {
    char ret = B.to_ulong();
    return ret;
}


// take an integer and split it into
std::vector<byte> ByteHelper::splitIntoBytes( const u_int64_t input, const int length ) {
    std::vector<byte> byteVector(length);
    for ( int i = 0; i < length; i++ ) {
        byteVector[length-i-1] = byte(input >> (i*8));
    }
    return byteVector;
}
