/*============================================================*/
/* mrfdataadvbase.cpp                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides advanced MRF data base class.                     */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdataadvbase.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfDataAdvBase::TMrfDataAdvBase(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfData(),
	_datastreamlength(0),
	_blocklength(blocklength),
	_defaultindex(defaultindex),
	_defaultstreamoffset(defaultstreamoffset),
	_defaultvalueoffset(defaultvalueoffset),
	_defaultreverse(defaultreverse),
	_defaultstreamreverse(defaultstreamreverse)
{
}

TMrfDataAdvBase::~TMrfDataAdvBase()
{
}

void TMrfDataAdvBase::setLocalConfItemValue(TConfItem& item)
{
	item.value = getStreamConfItemValue(item);
}

void TMrfDataAdvBase::setLocalConfItemValue(TConfItem& item, const u_int32_t& value)
{
	item.value = value;
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item)
{
	setStreamConfItemValue(item, item.value, _defaultindex, _defaultstreamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

const u_int32_t& TMrfDataAdvBase::getStreamConfItemValue(const TConfItem& item) const
{
	return getStreamConfItemValue(item, _defaultindex, _defaultstreamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item, const u_int32_t& value)
{
	setStreamConfItemValue(item, value, _defaultindex, _defaultstreamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

const u_int32_t& TMrfDataAdvBase::getStreamConfItemValue(const TConfItem& item, const u_int32_t& index) const
{
	return getStreamConfItemValue(item, index, _defaultstreamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index)
{
	setStreamConfItemValue(item, value, index, _defaultstreamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

const u_int32_t& TMrfDataAdvBase::getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset) const
{
	return getStreamConfItemValue(item, index, streamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset)
{
	setStreamConfItemValue(item, value, index, streamoffset, _defaultvalueoffset, _defaultreverse, _defaultstreamreverse);
}

const u_int32_t& TMrfDataAdvBase::getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset) const
{
	return getStreamConfItemValue(item, index, streamoffset, valueoffset, _defaultreverse, _defaultstreamreverse);
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset)
{
	setStreamConfItemValue(item, value, index, streamoffset, valueoffset, _defaultreverse, _defaultstreamreverse);
}

const u_int32_t& TMrfDataAdvBase::getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse) const
{
	return getStreamConfItemValue(item, index, streamoffset, valueoffset, reverse, _defaultstreamreverse);
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse)
{
	setStreamConfItemValue(item, value, index, streamoffset, valueoffset, reverse, _defaultstreamreverse);
}

const u_int32_t& TMrfDataAdvBase::getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const
{
	if (streamreverse) {
		return getBitBlock((index * _blocklength) + streamoffset - item.position - item.length, item.length, valueoffset, reverse);
	} else {
		return getBitBlock((index * _blocklength) + streamoffset + item.position, item.length, valueoffset, reverse);
	}
}

void TMrfDataAdvBase::setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse)
{
	if (streamreverse) {
		setBitBlock((index * _blocklength) + streamoffset - item.position - item.length, item.length, value, valueoffset, reverse);
	} else {
		setBitBlock((index * _blocklength) + streamoffset + item.position, item.length, value, valueoffset, reverse);
	}
}
