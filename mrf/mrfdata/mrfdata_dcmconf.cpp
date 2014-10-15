/*============================================================*/
/* mrfdata_dcmconf.cpp                                        */
/* MVD Readout Framework Hit Data Storage                     */
/* Specific to Virtex4 DCM_ADV Data Format                    */
/*                                               M.C. Mertens */
/*============================================================*/

#include <map>
#include <string>
#include "mrf_confitem.h"
#include "mrfdata_dcmconf.h"

TMrfData_DCMConf::TMrfData_DCMConf()
	: TMrfData()
{
	initMaps();
}


void TMrfData_DCMConf::initMaps()
{
	maplist.clear();
	initMultiplierStructure();
	initDividerStructure();
}

void TMrfData_DCMConf::initMultiplierStructure()
{
	maplist["Multiplier"].clear();
	maplist["Multiplier"]["DRPAddress"] = TConfItem(0x50, 16, 7);
	maplist["Multiplier"]["Value"] = TConfItem(0, 0, 16);
}

void TMrfData_DCMConf::initDividerStructure()
{
	maplist["Divider"].clear();
	maplist["Divider"]["DRPAddress"] = TConfItem(0x52, 16, 7);
	maplist["Divider"]["Value"] = TConfItem(0, 0, 16);
}

u_int32_t TMrfData_DCMConf::getItemValue(const std::string& source, const std::string& item) const
{
	return (maplist.find(source)->second.find(item)->second.value);
}

void TMrfData_DCMConf::setItemValue(const std::string& source, const std::string& item, const mrf::registertype& value)
{
	maplist.find(source)->second.find(item)->second.value = value;
}

void TMrfData_DCMConf::assemble(const std::string& source)
{
	clearDataStream();
	setNumWords(1);
	std::map<const std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(source); iter != getItemIteratorEnd(source); ++iter) {
		setBitBlock(iter->second.position, iter->second.length, iter->second.value, 0, false);
	}
}

std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator TMrfData_DCMConf::getTypeIteratorBegin() const
{
	return maplist.begin();
}

std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator TMrfData_DCMConf::getTypeIteratorEnd() const
{
	return maplist.end();
}


std::map<const std::string, TConfItem>::const_iterator TMrfData_DCMConf::getItemIteratorBegin(const std::string& type) const
{
	return maplist.find(type)->second.begin();
}

std::map<const std::string, TConfItem>::const_iterator TMrfData_DCMConf::getItemIteratorEnd(const std::string& type) const
{
	return maplist.find(type)->second.end();
}


