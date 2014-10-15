/*============================================================*/
/* mrfdata_chainltc.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides access to LTC DAC chain configuration.            */
/*                                                     S.Esch */
/*============================================================*/

#include "mrfdata_chainltc.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

TMrfData_ChainLTC::TMrfData_ChainLTC(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdvBase(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
        TMrfDataAdv2D(blocklength)
{

}

void TMrfData_ChainLTC::setCommands(const u_int32_t& command1, const u_int32_t& command2)
{
    setLocalItemValue("LTC1","Command",command1);
    setLocalItemValue("LTC2","Command",command2);
}
void TMrfData_ChainLTC::setCommand(const std::string& ltc, const u_int32_t& command)
{
        setLocalItemValue(ltc,"Command",command);
}
void TMrfData_ChainLTC::setDACValues(const std::string& dac1, const u_int32_t& value1,const std::string& dac2, const u_int32_t& value2)
{
        _ltcdacvalues.find("LTC1")->second.find(dac1)->second.value = value1;
        _ltcdacvalues.find("LTC2")->second.find(dac2)->second.value = value2;
}
void TMrfData_ChainLTC::setDACValue(const std::string& ltc,const std::string& dac, const u_int32_t& value)
{
        _ltcdacvalues.find(ltc)->second.find(dac)->second.value = value;
}

u_int32_t TMrfData_ChainLTC::getDACValue(const std::string& ltc, const std::string& dac)
{
    return _ltcdacvalues.find(ltc)->second.find(dac)->second.value;
}

void TMrfData_ChainLTC::setDACActivated(const std::string& ltc, const std::string& dac, const u_int32_t& active)
{
        _ltcdacactivated.find(ltc)->second.find(dac)->second.value = active;
}

u_int32_t TMrfData_ChainLTC::getDACActivated(const std::string& ltc, const std::string& dac)
{
        return _ltcdacactivated.find(ltc)->second.find(dac)->second.value;
}

void TMrfData_ChainLTC::assembleDAC(const std::string& dac)
{
	if (getNumBits() < _datastreamlength) {
		setNumBits(_datastreamlength);
	}
        setStreamItemValue("LTC1","Command");
        setStreamItemValue("LTC2","Command");

        setStreamConfItemValue(_ltcdacaddress.find("LTC1")->second.find(dac)->second);
        setStreamConfItemValue(_ltcdacvalues.find("LTC1")->second.find(dac)->second);

        setStreamConfItemValue(_ltcdacaddress.find("LTC2")->second.find(dac)->second);
        setStreamConfItemValue(_ltcdacvalues.find("LTC2")->second.find(dac)->second);
}


std::map<std::string, TConfItem>::iterator TMrfData_ChainLTC::getDACIteratorBegin(const std::string& ltc)
{
        return _ltcdacvalues.find(ltc)->second.begin();
}

std::map<std::string, TConfItem>::iterator TMrfData_ChainLTC::getDACIteratorEnd(const std::string& ltc)
{
        return _ltcdacvalues.find(ltc)->second.end();
}

std::map<std::string, TConfItem>::iterator TMrfData_ChainLTC::getDACActivatedIteratorBegin(const std::string& ltc)
{
        return _ltcdacactivated.find(ltc)->second.begin();
}

std::map<std::string, TConfItem>::iterator TMrfData_ChainLTC::getDACActivatedIteratorEnd(const std::string& ltc)
{
        return _ltcdacactivated.find(ltc)->second.end();
}

std::map<std::string, TConfItem>::const_iterator TMrfData_ChainLTC::getConstDACIteratorBegin(const std::string& ltc) const
{
        return _ltcdacvalues.find(ltc)->second.begin();
}

std::map<std::string, TConfItem>::const_iterator TMrfData_ChainLTC::getConstDACIteratorEnd(const std::string& ltc) const
{
        return _ltcdacvalues.find(ltc)->second.end();
}

std::map<std::string, TConfItem>::const_iterator TMrfData_ChainLTC::getConstDACActivatedIteratorBegin(const std::string& ltc) const
{
        return _ltcdacactivated.find(ltc)->second.begin();
}

std::map<std::string, TConfItem>::const_iterator TMrfData_ChainLTC::getConstDACActivatedIteratorEnd(const std::string& ltc) const
{
        return _ltcdacactivated.find(ltc)->second.end();
}
std::map<std::string, std::map< std::string, TConfItem> >::iterator TMrfData_ChainLTC::getLTCIteratorBegin()
{
        return _ltcdacvalues.begin();
}
std::map<std::string, std::map< std::string, TConfItem> >::iterator TMrfData_ChainLTC::getLTCIteratorEnd()
{
        return _ltcdacvalues.end();
}

std::map<std::string, std::map< std::string, TConfItem> >::iterator TMrfData_ChainLTC::getLTCActivatedIteratorBegin()
{
        return _ltcdacactivated.begin();
}
std::map<std::string, std::map< std::string, TConfItem> >::iterator TMrfData_ChainLTC::getLTCActivatedIteratorEnd()
{
        return _ltcdacactivated.end();
}


int TMrfData_ChainLTC::getDACCount(const std::string& ltc) const
{
        return _ltcdacvalues.find(ltc)->second.size();
}
