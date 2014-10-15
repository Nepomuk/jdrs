/*============================================================*/
/* mrfcal_topix.cpp                                           */
/* MVD Readout Framework Chip Access Layer                    */
/* Chip Access Layer for Topix Frontend Prototype             */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfcal_topix.h"
#include "mrftal_rbtpx.h"
#include "mrfcal.h"
#include "mrfdata/mrfdata_ltc.h"
#include "mrfdata/mrfdata_tpxmasterreg.h"
#include <map>

TMrfCal_Topix::TMrfCal_Topix()
: TMrfCal(), TMrfTal_RBTopix()
{
}

TMrfCal_Topix::~TMrfCal_Topix()
{
}

void TMrfCal_Topix::configLTC(TMrfData_LTC2620 data) const
{
	for (std::map<const std::string, TConfItem>::const_iterator iter = data.getDACActivatedIteratorBegin(); iter != data.getDACActivatedIteratorEnd(); ++iter) {
		if (iter->second.value == 1) {
			//data.setCommand(ltc2620::writeupdate);
			data.setCommand(ltc2620::write);
		} else {
			data.setCommand(ltc2620::powerdown);
		}
		data.assembleDAC(iter->first);
		writeLTCData(data);
	}
	for (std::map<const std::string, TConfItem>::const_iterator iter = data.getDACActivatedIteratorBegin(); iter != data.getDACActivatedIteratorEnd(); ++iter) {
		if (iter->second.value == 1) {
			data.setCommand(ltc2620::update);
			data.assembleDAC(iter->first);
			writeLTCData(data);
		}
	}

	/*
	data.setCommand(ltc2620::write);
	data.assembleDAC("DACA");
	writeLTCData(data);
	data.assembleDAC("DACB");
	writeLTCData(data);
	data.assembleDAC("DACC");
	writeLTCData(data);
	data.assembleDAC("DACD");
	writeLTCData(data);
	data.assembleDAC("DACE");
	writeLTCData(data);
	data.assembleDAC("DACF");
	writeLTCData(data);
	data.setCommand(ltc2620::writeupdateall);
	data.assembleDAC("DACG");
	writeLTCData(data);
	*/
}

void TMrfCal_Topix::prepareMaster(TMrfData_TpxMasterReg data) const
{
	for (std::map<const std::string, TConfItem>::const_iterator iter = data.getConstItemIteratorBegin(); iter != data.getConstItemIteratorEnd(); ++iter) {
		writeRegister(data, iter->first);
	}
}

void TMrfCal_Topix::writeToChip(const TMrfData& data) const
{
	writeRemoteData(data);
	startShifter(data.getNumWords());
}

void TMrfCal_Topix::readFromChip(TMrfData& data)
{
	triggerRead(data.getNumWords(), true);
	readRemoteData(data);
}
