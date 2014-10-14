/*============================================================*/
/* mrfdata_tpxflags.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Control Flags.                    */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpx3flags.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfData_Tpx3Flags::TMrfData_Tpx3Flags(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_Tpx3Flags::initMaps()
{
	_localdata.clear();
        _localdata["Bit0"]           = TConfItem(0, 0, 1);
        _localdata["Bit1"]           = TConfItem(0, 1, 1);
        _localdata["Bit2"]           = TConfItem(0, 2, 1);
        _localdata["Bit3"]           = TConfItem(0, 3, 1);
        _localdata["Bit4"]           = TConfItem(0, 4, 1);
        _localdata["Bit5"]           = TConfItem(0, 5, 1);
        _localdata["TestPin"]        = TConfItem(0, 6, 1);
        _localdata["CounterReset"]   = TConfItem(0, 7, 1);
        _localdata["LTCClearBar"]    = TConfItem(0, 8, 1);
        _localdata["TopixSel0"]      = TConfItem(0, 9, 1);
        _localdata["TopixSel1"]      = TConfItem(0,10, 1);
        _localdata["TopixReset"]     = TConfItem(0,11, 1);
        _localdata["Bit12"]          = TConfItem(0,12, 1);
        _localdata["Bit13"]          = TConfItem(0,13, 1);
        _localdata["Bit14"]          = TConfItem(0,14, 1);
        _localdata["Bit15"]          = TConfItem(0,15, 1);
        _localdata["Bit16"]          = TConfItem(0,16, 1);
        _localdata["Bit17"]          = TConfItem(0,17, 1);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}
