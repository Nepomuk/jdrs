/*============================================================*/
/* mrftools.h                                                 */
/* Toolbox                                                    */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFTOOLS_H__
#define __MRFTOOLS_H__

#include <sys/types.h>
#include <map>
#include <string>
#include "mrf_confitem.h"

//! Helper functions for single bit manipulations.
namespace mrftools {

//! Retrieves a single bit from an integer value.
/*!
\param position Position of bit to investigate.
\param value Value whose bits are to be investigated.
\return \b True if the bit is set, \b False if the bit is not set.
*/
bool getIntBit(const u_int32_t& position, const u_int32_t& value);

//! Sets a single bit in an integer value.
/*!
\param position The positional index of the bit to be manipulated, 0 referring to the least significant bit.
\param value The integer value whose bits are to be manipulated.
\param state \b True sets the bit, \b False resets the bit.
*/
void setIntBit(const u_int32_t& position, u_int32_t& value, const bool& state);

//! Shifts the bits in an integer value.
/*!
\param positions Number of positions to be shifted. A positive value means shift to the MSB (left), negative means shift to the LSB (right).
\param value The integer value whose bits are to be shifted.
\return The original value shifted by the given number of positions.
*/
u_int32_t shiftBy(const int& positions, const u_int32_t& value);

unsigned int getIteratorItemCount(const std::map<std::string, TConfItem>::const_iterator& start, const std::map<std::string, TConfItem>::const_iterator& stop);

}


#endif // __MRFTOOLS_H__

