/*============================================================*/
/* mrfdata_tpxflags.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Control Flags.                    */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpxflags.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfData_TpxFlags::TMrfData_TpxFlags(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_TpxFlags::initMaps()
{
	_localdata.clear();
	_localdata["BusSel1"] = TConfItem(0, 0, 1);
	_localdata["BusSel0"] = TConfItem(0, 1, 1);
	_localdata["ColSel1"] = TConfItem(0, 2, 1);
	_localdata["ColSel0"] = TConfItem(0, 3, 1);
	_localdata["CounterEnable"] = TConfItem(0, 4, 1);
	_localdata["GrayEnable"] = TConfItem(0, 5, 1);
	_localdata["Reset"] = TConfItem(0, 6, 1);
	_localdata["Freeze"] = TConfItem(0, 7, 1);
	_localdata["LTCClear"] = TConfItem(0, 8, 1);
	_localdata["ReadCMD"] = TConfItem(0, 9, 1);
	_localdata["ReadTE"] = TConfItem(0, 10, 1);
	_localdata["ReadLE"] = TConfItem(0, 11, 1);
	_localdata["APrecharge"] = TConfItem(0, 12, 1);
	_localdata["DPrecharge"] = TConfItem(0, 13, 1);
	_localdata["ALatch"] = TConfItem(0, 14, 1);
	_localdata["DLatch"] = TConfItem(0, 15, 1);
	_localdata["ConfigMode"] = TConfItem(0, 16, 1);
	_localdata["ExtTrigger"] = TConfItem(0, 17, 1);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}
