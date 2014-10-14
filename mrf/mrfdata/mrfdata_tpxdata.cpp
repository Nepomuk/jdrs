/*============================================================*/
/* mrfdata_tpxpixel.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpxdata.h"
#include "mrf_confitem.h"
#include "mrfdataadv1d.h"

TMrfData_TpxData::TMrfData_TpxData(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_TpxData::initMaps()
{
	_localdata.clear();
	_localdata["LoWord"] = TConfItem(0, 0, 12);
	_localdata["HiWord"] = TConfItem(0, 12, 12);
	_localdata["Padding"] = TConfItem(0, 24, 8);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}


u_int32_t TMrfData_TpxData::getHiWord(const unsigned int& index) const
{
	return getStreamItemValue("HiWord", index);
}

u_int32_t TMrfData_TpxData::getLoWord(const unsigned int& index) const
{
	return getStreamItemValue("LoWord", index);
}

void TMrfData_TpxData::setHiWord(const unsigned int& index, const u_int32_t& value)
{
	setStreamItemValue("HiWord", value, index);
}

void TMrfData_TpxData::setLoWord(const unsigned int& index, const u_int32_t& value)
{
	setStreamItemValue("LoWord", value, index);
}
