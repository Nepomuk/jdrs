/*============================================================*/
/* mrfdataadv1d.cpp                                           */
/* MVD Readout Framework Data Storage                         */
/* Provides generic 1D TConfItemAddress map and accessors.    */
/*                                               S.Esch		  */
/*============================================================*/

#include "mrfdataadv1daddress.h"
#include <string>

TMrfDataAdv1DAddress::TMrfDataAdv1DAddress(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
}

TMrfDataAdv1DAddress::~TMrfDataAdv1DAddress()
{
}


const u_int32_t& TMrfDataAdv1DAddress::getLocalItemAddress(const std::string& item) const
{
	return _localdata.find(item)->second.position;

}
//TMrfDataAdv1D(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse);
