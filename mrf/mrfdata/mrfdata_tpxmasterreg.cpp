/*============================================================*/
/* mrfdata_tpxmasterreg.cpp                                   */
/* MVD Readout Framework Data Storage                         */
/* Provides access to RB Topix Master Control  .              */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpxmasterreg.h"
#include <map>
#include <string>
#include "mrf_confitem.h"
#include "mrftal_rbtpx.h"

TMrfData_TpxMasterReg::TMrfData_TpxMasterReg(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

TMrfData_TpxMasterReg::~TMrfData_TpxMasterReg()
{
}

void TMrfData_TpxMasterReg::initMaps()
{
	_localdata.clear();
	_localdata["BusSel1"] = TConfItem(0, 0, 32, tpx_address::bussel1data);
	_localdata["BusSel0"] = TConfItem(0, 0, 32, tpx_address::bussel0data);
	_localdata["APrecharge"] = TConfItem(0, 0, 32, tpx_address::aprechrgdata);
	_localdata["DPrecharge"] = TConfItem(0, 0, 32, tpx_address::dprechrgdata);
	_localdata["ALatch"] = TConfItem(0, 0, 32, tpx_address::alatchdata);
	_localdata["DLatch"] = TConfItem(0, 0, 32, tpx_address::dlatchdata);
	_localdata["ReadCMD"] = TConfItem(0, 0, 32, tpx_address::readcmddata);
	_localdata["ReadLE"] = TConfItem(0, 0, 32, tpx_address::readledata);
	_localdata["ReadTE"] = TConfItem(0, 0, 32, tpx_address::readtedata);
	_localdata["TriggerLo"] = TConfItem(0, 0, 32, tpx_address::triggerlodata);
	_localdata["TriggerHi"] = TConfItem(0, 0, 32, tpx_address::triggerhidata);
	_localdata["Bitcount"] = TConfItem(0, 0, 32, tpx_address::masterbitcount);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}
