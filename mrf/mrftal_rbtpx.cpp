/*============================================================*/
/* mrftal_rbtpx.cpp                                           */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* including ToPiX protocol                                   */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrftal_rbtpx.h"
#include "mrfdata_ltc2620.h"
#include <iostream>

TMrfTal_RBTopix::TMrfTal_RBTopix()
: TMrfTal_RBUDP()
{
}

TMrfTal_RBTopix::~TMrfTal_RBTopix()
{
}

void TMrfTal_RBTopix::writeRemoteData(const TMrfData& data) const
{
	writeData(tpx_address::input, data);
}

void TMrfTal_RBTopix::readRemoteData(TMrfData& data) const
{
	readOutputBuffer(data, data.getNumWords());
}

void TMrfTal_RBTopix::triggerRead(const u_int32_t& triggercount, const bool& withlo) const
{
	write(tpx_address::triggercount, triggercount);
	boardCommand(tpxctrl_value::triggerwithlo, withlo);
	boardCommand(tpxctrl_value::triggerread);
}

void TMrfTal_RBTopix::configTopixSlowReg(const TMrfData_TpxFlags& data) const
{
	writeRegisterData(tpx_address::flags, data);
}

void TMrfTal_RBTopix::writeLTCData(const TMrfData_LTC& data) const
{
        std::cerr << "data.getNumBits()" << data.getNumBits() << std::endl;
        std::cerr << "data.exportBinString()" << data.exportBinString() << std::endl;
	writeRegisterData(tpx_address::ltcconfig, data);
	boardCommand(tpxctrl_value::ltcconfig);

}

void TMrfTal_RBTopix::startShifter(const u_int32_t& count) const
{
	write(tpx_address::inputcount, count);
	boardCommand(tpxctrl_value::startshifter);
}

void TMrfTal_RBTopix::startMaster() const
{
	boardCommand(tpxctrl_value::startmaster);
}
