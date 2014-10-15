/*============================================================*/
/* mrfdata_ltc2620.cpp                                        */
/* MVD Readout Framework Data Storage                         */
/* Provides access to LTC2620 DAC configuration.              */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_ltc2620.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfData_LTC2620::TMrfData_LTC2620(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdvBase(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
  TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
  TMrfData_LTC(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_LTC2620::initMaps()
{
	_localdata.clear();
	_localdata["Padding"] = TConfItem(0, 0, 8);
	_localdata["Command"] = TConfItem(0, 8, 4);
	_localdata["Address"] = TConfItem(0, 12, 4);
	_localdata["Data"] = TConfItem(0, 16, 12);
	_localdata["DontCare"] = TConfItem(0, 28, 4);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}

	_ltcdacvalues.clear();
	_ltcdacvalues["DACA"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACB"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACC"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACD"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACE"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACF"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACG"] = TConfItem(0, 16, 12);
	_ltcdacvalues["DACH"] = TConfItem(0, 16, 12);

	_ltcdacactivated.clear();
	_ltcdacactivated["DACA"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACB"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACC"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACD"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACE"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACF"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACG"] = TConfItem(0, 0, 1);
	_ltcdacactivated["DACH"] = TConfItem(0, 0, 1);

	_ltcdacaddress.clear();
	_ltcdacaddress["DACA"] = TConfItem(ltc2620::daca, 12, 4);
	_ltcdacaddress["DACB"] = TConfItem(ltc2620::dacb, 12, 4);
	_ltcdacaddress["DACC"] = TConfItem(ltc2620::dacc, 12, 4);
	_ltcdacaddress["DACD"] = TConfItem(ltc2620::dacd, 12, 4);
	_ltcdacaddress["DACE"] = TConfItem(ltc2620::dace, 12, 4);
	_ltcdacaddress["DACF"] = TConfItem(ltc2620::dacf, 12, 4);
	_ltcdacaddress["DACG"] = TConfItem(ltc2620::dacg, 12, 4);
	_ltcdacaddress["DACH"] = TConfItem(ltc2620::dach, 12, 4);
}

//void TMrfData_LTC2620::setCommand(const u_int32_t& command)
//{
//	setLocalItemValue("Command", command);
//}

//void TMrfData_LTC2620::setDACValue(const std::string& dac, const u_int32_t& value)
//{
//	_ltcdacvalues.find(dac)->second.value = value;
//}

//u_int32_t TMrfData_LTC2620::getDACValue(const std::string& dac)
//{
//	return _ltcdacvalues.find(dac)->second.value;
//}

//void TMrfData_LTC2620::setDACActivated(const std::string& dac, const u_int32_t& active)
//{
//	_ltcdacactivated.find(dac)->second.value = active;
//}

//u_int32_t TMrfData_LTC2620::getDACActivated(const std::string& dac)
//{
//	return _ltcdacactivated.find(dac)->second.value;
//}

//void TMrfData_LTC2620::assembleDAC(const std::string& dac)
//{
//	if (getNumBits() < _datastreamlength) {
//		setNumBits(_datastreamlength);
//	}
//	setStreamItemValue("Command");
//	setStreamConfItemValue(_ltcdacaddress.find(dac)->second);
//	setStreamConfItemValue(_ltcdacvalues.find(dac)->second);
//}

//std::map<std::string, TConfItem>::iterator TMrfData_LTC2620::getDACIteratorBegin()
//{
//	return _ltcdacvalues.begin();
//}

//std::map<std::string, TConfItem>::iterator TMrfData_LTC2620::getDACIteratorEnd()
//{
//	return _ltcdacvalues.end();
//}

//std::map<std::string, TConfItem>::iterator TMrfData_LTC2620::getDACActivatedIteratorBegin()
//{
//	return _ltcdacactivated.begin();
//}

//std::map<std::string, TConfItem>::iterator TMrfData_LTC2620::getDACActivatedIteratorEnd()
//{
//	return _ltcdacactivated.end();
//}

//std::map<std::string, TConfItem>::const_iterator TMrfData_LTC2620::getConstDACIteratorBegin() const
//{
//	return _ltcdacvalues.begin();
//}

//std::map<std::string, TConfItem>::const_iterator TMrfData_LTC2620::getConstDACIteratorEnd() const
//{
//	return _ltcdacvalues.end();
//}

//std::map<std::string, TConfItem>::const_iterator TMrfData_LTC2620::getConstDACActivatedIteratorBegin() const
//{
//	return _ltcdacactivated.begin();
//}

//std::map<std::string, TConfItem>::const_iterator TMrfData_LTC2620::getConstDACActivatedIteratorEnd() const
//{
//	return _ltcdacactivated.end();
//}

//int TMrfData_LTC2620::getDACCount() const
//{
//	return _ltcdacvalues.size();
//}
