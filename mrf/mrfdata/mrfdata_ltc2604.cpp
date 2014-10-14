/*============================================================*/
/* mrfdata_ltc2620.cpp                                        */
/* MVD Readout Framework Data Storage                         */
/* Provides access to LTC2620 DAC configuration.              */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_ltc2604.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfData_LTC2604::TMrfData_LTC2604(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdvBase(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
  TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
  TMrfData_LTC(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_LTC2604::initMaps()
{
	_localdata.clear();
	_localdata["Padding"] = TConfItem(0, 0, 8);
	_localdata["Command"] = TConfItem(0, 8, 4);
	_localdata["Address"] = TConfItem(0, 12, 4);
        _localdata["Data"] = TConfItem(0, 16, 16);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}

	_ltcdacvalues.clear();
        _ltcdacvalues["DACA"] = TConfItem(0, 16, 16);
        _ltcdacvalues["DACB"] = TConfItem(0, 16, 16);
        _ltcdacvalues["DACC"] = TConfItem(0, 16, 16);
        _ltcdacvalues["DACD"] = TConfItem(0, 16, 16);

	_ltcdacactivated.clear();
        _ltcdacactivated["DACA"] = TConfItem(0, 0, 1);
        _ltcdacactivated["DACB"] = TConfItem(0, 0, 1);
        _ltcdacactivated["DACC"] = TConfItem(0, 0, 1);
        _ltcdacactivated["DACD"] = TConfItem(0, 0, 1);

	_ltcdacaddress.clear();
        _ltcdacaddress["DACA"] = TConfItem(ltc2604::daca, 12, 4);
        _ltcdacaddress["DACB"] = TConfItem(ltc2604::dacb, 12, 4);
        _ltcdacaddress["DACC"] = TConfItem(ltc2604::dacc, 12, 4);
        _ltcdacaddress["DACD"] = TConfItem(ltc2604::dacd, 12, 4);
}
