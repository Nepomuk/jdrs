/*============================================================*/
/* mrf_confitem.cpp                                           */
/* MVD Readout Framework Configuration Item                   */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrf_confitem.h"


TConfItem::TConfItem()
: value(0), position(0), length(0), min(0), max(0), flags(0)
{
}

/*
TConfItem::TConfItem(const mrf::registertype value, const u_int32_t position, const u_int32_t length)
: value(value), position(position), length(length), user1(0), user2(0)
{
}
*/

TConfItem::TConfItem(const mrf::registertype& Value, const u_int32_t& Position, const u_int32_t& Length, const u_int32_t& Flags)
: value(Value), position(Position), length(Length), min(0), max((1 << length) - 1), flags(Flags)
{
}

TConfItem::TConfItem(const mrf::registertype& Value, const u_int32_t& Position, const u_int32_t& Length, const u_int32_t& Min, const u_int32_t& Max, const u_int32_t& Flags)
: value(Value), position(Position), length(Length), min(Min), max(Max), flags(Flags)
{
}



