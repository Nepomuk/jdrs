/*============================================================*/
/* mrftools.cpp                                               */
/* Toolbox                                                    */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrftools.h"
#include <climits>

namespace mrftools {

static const u_int32_t u_int32_t_bitlength = sizeof(u_int32_t) * CHAR_BIT;

bool getIntBit(const u_int32_t& position, const u_int32_t& value)
{
	if (position < u_int32_t_bitlength) {
		return (value & (1 << position));
	}
	else {
		return false;
	}
}

void setIntBit(const u_int32_t& position, u_int32_t& value, const bool& state)
{
	if (position < u_int32_t_bitlength) {
		if (state) {
			value |= (1 << position);
		}
		else {
			value &= (~(1 << position));
		}
	}
}

u_int32_t shiftBy(const int& positions, const u_int32_t& value)
{
	if (positions < 0) {
		return (value >> (-positions));
	}
	else if (positions > 0) {
		return (value << positions);
	}
	else {
		return value;
	}
}

unsigned int getIteratorItemCount(const std::map<std::string, TConfItem>::const_iterator& start, const std::map<std::string, TConfItem>::const_iterator& stop)
{
	std::map<std::string, TConfItem>::const_iterator iter;
	unsigned int count = 0;
	for (iter = start; iter != stop; ++iter) {
		++count;
	}
	return count;
}

}

