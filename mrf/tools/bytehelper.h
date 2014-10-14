/*============================================================*/
/* tools/bytehelper.h                                         */
/* A collection of helper functions to make life with bytes   */
/* easier.                                                    */
/*                                                 A. Goerres */
/*============================================================*/

#ifndef BYTEHELPER_H
#define BYTEHELPER_H

#include <stdlib.h>
#include <bitset>
#include <vector>
#include <string.h>


//! Define a byte and a vector of bytes to make things more easy
typedef std::bitset<8> byte;
typedef std::vector<byte> byte_v;


//! The ByteHelper class
/*!
  This class provides some functions, that make life
  */
class ByteHelper
{
  public:
    ByteHelper();

    std::string bytes2HexString(const byte_v bytes);
    std::string bytes2BitString(const byte_v bytes);
    char byte2Char(const byte B);
    std::vector<byte> splitIntoBytes(const u_int64_t input, const int length);
};

#endif // BYTEHELPER_H
