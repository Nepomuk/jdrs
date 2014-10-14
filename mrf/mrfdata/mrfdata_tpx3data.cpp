/*============================================================*/
/* mrfdata_tpxpixel.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpx3data.h"
#include "mrf_confitem.h"
#include "mrfdataadv1d.h"

TMrfData_Tpx3Data::TMrfData_Tpx3Data(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_Tpx3Data::initMaps()
{
	_localdata.clear();
        _localdata["columnaddress"] = TConfItem(0, 0, 1);
        _localdata["pixeladdress"] = TConfItem(0, 1, 7);
        _localdata["leadingedge"] = TConfItem(0, 8, 12);
        _localdata["trailingedge"] = TConfItem(0, 20, 12);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}



