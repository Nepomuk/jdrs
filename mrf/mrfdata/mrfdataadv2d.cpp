/*============================================================*/
/* mrfdataadv2d.cpp                                           */
/* MVD Readout Framework Data Storage                         */
/* Provides generic 2D TConfItem map and accessors.           */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdataadv2d.h"
#include <map>
#include <string>
#include "mrf_confitem.h"
#include <iostream>

TMrfDataAdv2D::TMrfDataAdv2D(const u_int32_t& blocklength)
: TMrfDataAdvBase(blocklength)
{
}

TMrfDataAdv2D::~TMrfDataAdv2D()
{
}

const u_int32_t& TMrfDataAdv2D::getLocalItemValue(const std::string& source, const std::string& item) const
{
	return _localdata.find(source)->second.find(item)->second.value;
}

void TMrfDataAdv2D::setLocalItemValue(const std::string& source, const std::string& item, const u_int32_t& value)
{
	_localdata.find(source)->second.find(item)->second.value = value;
}

const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item)
{
    setStreamConfItemValue(_localdata.find(source)->second.find(item)->second);
}

const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second, index);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value, index);
}

const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second, index, streamoffset);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value, index, streamoffset);
}

const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second, index, streamoffset, valueoffset);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value, index, streamoffset, valueoffset);
}

const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second, index, streamoffset, valueoffset, reverse);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value, index, streamoffset, valueoffset, reverse);
}

const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second, index, streamoffset, valueoffset, reverse, streamreverse);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value, index, streamoffset, valueoffset, reverse, streamreverse);
}

/*
const u_int32_t& TMrfDataAdv2D::getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const
{
	return getStreamConfItemValue(_localdata.find(source)->second.find(item)->second, index, streamoffset, valueoffset, reverse, streamreverse);
}

void TMrfDataAdv2D::setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse)
{
	setStreamConfItemValue(_localdata.find(source)->second.find(item)->second, value, index, streamoffset, valueoffset, reverse, streamreverse);
}
*/

void TMrfDataAdv2D::assemble()
{
	if (getNumBits() < _datastreamlength) {
		setNumBits(_datastreamlength);
	}

        std::cout << "getNumBits()" << getNumBits() << std::endl;
        std::cout << "_datastreamlength" << _datastreamlength << std::endl;
	std::map<std::string, std::map<std::string, TConfItem> >::const_iterator iter;
	std::map<std::string, TConfItem>::const_iterator iter2;
	for (iter = getConstTypeIteratorBegin(); iter != getConstTypeIteratorEnd(); ++iter) {
		for (iter2 = getConstItemIteratorBegin(iter); iter2 != getConstItemIteratorEnd(iter); ++iter2) {
			setStreamConfItemValue(iter2->second);
		}
	}
}

void TMrfDataAdv2D::disassemble()
{
	std::map<std::string, std::map<std::string, TConfItem> >::iterator iter;
	std::map<std::string, TConfItem>::iterator iter2;
	for (iter = getTypeIteratorBegin(); iter != getTypeIteratorEnd(); ++iter) {
		for (iter2 = getItemIteratorBegin(iter); iter2 != getItemIteratorEnd(iter); ++iter2) {
			setLocalConfItemValue(iter2->second);
		}
	}
}

int TMrfDataAdv2D::getTypeCount() const
{
	return _localdata.size();
}

int TMrfDataAdv2D::getItemCount(const std::string& type) const
{
	return _localdata.find(type)->second.size();
}

TMrfDataAdv2D::constTypeIterator TMrfDataAdv2D::getConstTypeIteratorBegin() const
{
	return _localdata.begin();
}

TMrfDataAdv2D::constTypeIterator TMrfDataAdv2D::getConstTypeIteratorEnd() const
{
	return _localdata.end();
}

TMrfDataAdv2D::constItemIterator TMrfDataAdv2D::getConstItemIteratorBegin(const std::string& type) const
{
	return _localdata.find(type)->second.begin();
}

TMrfDataAdv2D::constItemIterator TMrfDataAdv2D::getConstItemIteratorEnd(const std::string& type) const
{
	return _localdata.find(type)->second.end();
}

TMrfDataAdv2D::constItemIterator TMrfDataAdv2D::getConstItemIteratorBegin(const std::map<std::string, std::map<std::string, TConfItem> >::const_iterator& type) const
{
	return type->second.begin();
}
TMrfDataAdv2D::constItemIterator TMrfDataAdv2D::getConstItemIteratorEnd(const std::map<std::string, std::map<std::string, TConfItem> >::const_iterator& type) const
{
	return type->second.end();
}

TMrfDataAdv2D::typeIterator TMrfDataAdv2D::getTypeIteratorBegin()
{
	return _localdata.begin();
}

TMrfDataAdv2D::typeIterator TMrfDataAdv2D::getTypeIteratorEnd()
{
	return _localdata.end();
}

TMrfDataAdv2D::itemIterator TMrfDataAdv2D::getItemIteratorBegin(const std::string& type)
{
	return _localdata.find(type)->second.begin();
}

TMrfDataAdv2D::itemIterator TMrfDataAdv2D::getItemIteratorEnd(const std::string& type)
{
	return _localdata.find(type)->second.end();
}

TMrfDataAdv2D::itemIterator TMrfDataAdv2D::getItemIteratorBegin(const std::map<std::string, std::map<std::string, TConfItem> >::iterator& type)
{
	return type->second.begin();
}
TMrfDataAdv2D::itemIterator TMrfDataAdv2D::getItemIteratorEnd(const std::map<std::string, std::map<std::string, TConfItem> >::iterator& type)
{
	return type->second.end();
}
