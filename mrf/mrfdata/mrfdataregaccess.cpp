/*============================================================*/
/* mrfdataregaccess.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides access RB Data Registers           .              */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdataregaccess.h"

//#include <map>
#include <string>

TMrfDataRegAccess::TMrfDataRegAccess(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
}

TMrfDataRegAccess::~TMrfDataRegAccess()
{
}

void TMrfDataRegAccess::setValue(const std::string& targetreg, const u_int32_t& value)
{
	setLocalItemValue(targetreg, value);
}

u_int32_t TMrfDataRegAccess::getValue(const std::string& targetreg) const
{
	return getLocalItemValue(targetreg);
}

void TMrfDataRegAccess::setAddress(const std::string& targetreg, const u_int32_t& address)
{
	_localdata.find(targetreg)->second.flags = address;
}

u_int32_t TMrfDataRegAccess::getAddress(const std::string& targetreg) const
{
	return _localdata.find(targetreg)->second.flags;
}
