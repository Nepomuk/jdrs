/*============================================================*/
/* mrf_confitem.h                                             */
/* MVD Readout Framework Configuration Item with Address      */
/*                                               S.Esch		  */
/*============================================================*/

#include "mrf_confitemaddress.h"

TConfItemAddress::TConfItemAddress()
: TConfItem(), address(0)
{

}

TConfItemAddress::~TConfItemAddress()
{

}

TConfItemAddress::TConfItemAddress(const mrf::registertype& Value, const u_int32_t& Position, const u_int32_t& Length, const u_int32_t& Flags, const u_int32_t& Address)
: TConfItem(Value, Position, Length, Flags), address(Address)
{

}
TConfItemAddress::TConfItemAddress(const mrf::registertype& Value, const u_int32_t& Position, const u_int32_t& Length,  const u_int32_t& Min, const u_int32_t& Max, const u_int32_t& Flags, const u_int32_t& Address)
: TConfItem(Value, Position, Length, Min, Max, Flags), address(Address)
{

}

