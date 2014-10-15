/*============================================================*/
/* mrfdataadv1d.cpp                                           */
/* MVD Readout Framework Data Storage                         */
/* Provides generic 1D TConfItem map and accessors.           */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdataadv1d.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfDataAdv1D::TMrfDataAdv1D(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdvBase(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
}

TMrfDataAdv1D::~TMrfDataAdv1D()
{
}

const u_int32_t& TMrfDataAdv1D::getLocalItemValue(const std::string& item) const
{
	return _localdata.find(item)->second.value;
}

void TMrfDataAdv1D::setLocalItemValue(const std::string& item, const u_int32_t& value)
{
	_localdata.find(item)->second.value = value;
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item)
{
	setStreamConfItemValue(_localdata.find(item)->second);
}

const u_int32_t& TMrfDataAdv1D::getStreamItemValue(const std::string& item) const
{
	return getStreamConfItemValue(_localdata.find(item)->second);
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item, const u_int32_t& value)
{
	setStreamConfItemValue(_localdata.find(item)->second, value);
}

const u_int32_t& TMrfDataAdv1D::getStreamItemValue(const std::string& item, const u_int32_t& index) const
{
	return getStreamConfItemValue(_localdata.find(item)->second, index);
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index)
{
	setStreamConfItemValue(_localdata.find(item)->second, value, index);
}

const u_int32_t& TMrfDataAdv1D::getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset) const
{
	return getStreamConfItemValue(_localdata.find(item)->second, index, streamoffset);
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset)
{
	setStreamConfItemValue(_localdata.find(item)->second, value, index, streamoffset);
}

const u_int32_t& TMrfDataAdv1D::getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset) const
{
	return getStreamConfItemValue(_localdata.find(item)->second, index, streamoffset, valueoffset);
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset)
{
	setStreamConfItemValue(_localdata.find(item)->second, value, index, streamoffset, valueoffset);
}

const u_int32_t& TMrfDataAdv1D::getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse) const
{
	return getStreamConfItemValue(_localdata.find(item)->second, index, streamoffset, valueoffset, reverse);
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse)
{
	setStreamConfItemValue(_localdata.find(item)->second, value, index, streamoffset, valueoffset, reverse);
}

const u_int32_t& TMrfDataAdv1D::getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const
{
	return getStreamConfItemValue(_localdata.find(item)->second, index, streamoffset, valueoffset, reverse, streamreverse);
}

void TMrfDataAdv1D::setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse)
{
	setStreamConfItemValue(_localdata.find(item)->second, value, index, streamoffset, valueoffset, reverse, streamreverse);
}

void TMrfDataAdv1D::assemble()
{
	if (getNumBits() < _datastreamlength) {
		setNumBits(_datastreamlength);
	}
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getConstItemIteratorBegin(); iter != getConstItemIteratorEnd(); ++iter) {
		setStreamConfItemValue(iter->second);
	}
}

void TMrfDataAdv1D::disassemble()
{
	std::map<std::string, TConfItem>::iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		//setLocalItemValue(iter->first, getStreamConfItemValue(iter->second));
		setLocalConfItemValue(iter->second);
		//setLocalConfItemValue(iter->second, getStreamConfItemValue(iter->second));
		//setCommandRegValue(iter->first, getBitBlock(mrf_afei3_reglength::command - iter->second.position - iter->second.length, iter->second.length, 0, true));
	}

}

int TMrfDataAdv1D::getItemCount() const
{
	return _localdata.size();
}

TMrfDataAdv1D::constItemIterator TMrfDataAdv1D::getConstItemIteratorBegin() const
{
	return _localdata.begin();
}

TMrfDataAdv1D::constItemIterator TMrfDataAdv1D::getConstItemIteratorEnd() const
{
	return _localdata.end();
}

TMrfDataAdv1D::itemIterator TMrfDataAdv1D::getItemIteratorBegin()
{
	return _localdata.begin();
}

TMrfDataAdv1D::itemIterator TMrfDataAdv1D::getItemIteratorEnd()
{
	return _localdata.end();
}
