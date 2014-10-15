/*============================================================*/
/* mrfdata_tpxpixel.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpx3command.h"
#include "mrf_confitem.h"
#include "mrfdataadv1d.h"

TMrfData_Tpx3Command::TMrfData_Tpx3Command(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_Tpx3Command::initMaps()
{
	_localdata.clear();
        _localdata["data"]              = TConfItem(0, 0,12);
        _localdata["operationcode"]     = TConfItem(0,12, 5);
        _localdata["padding"]           = TConfItem(0,17,15);


	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}



