/*============================================================*/
/* mrftal_rbtpx.cpp                                           */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* including ToPiX protocol                                   */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrftal_rbtpx3.h"
#include "mrfdata_ltc2604.h"
#include "mrfdata_chain2ltc2604.h"
#include <iostream>

TMrfTal_RBTopix3::TMrfTal_RBTopix3()
: TMrfTal_RBUDP()
{
}

TMrfTal_RBTopix3::~TMrfTal_RBTopix3()
{
}

void TMrfTal_RBTopix3::writeRemoteData(const TMrfData& data) const
{
        std::cerr <<"data.getNumBits() "<< data.getNumBits() << std::endl;
        std::cerr <<"data.exportBinString() "<< data.exportBinString() << std::endl;
        writeData(tpx_address::input, data);
}

void TMrfTal_RBTopix3::readRemoteData(TMrfData& data) const
{
	readOutputBuffer(data, data.getNumWords());
}

void TMrfTal_RBTopix3::triggerRead(const u_int32_t& triggercount, const bool& withlo) const
{
	write(tpx_address::triggercount, triggercount);
	boardCommand(tpxctrl_value::triggerwithlo, withlo);
	boardCommand(tpxctrl_value::triggerread);
}

void TMrfTal_RBTopix3::configTopixSlowReg(const TMrfData_Tpx3Flags& data) const
{
        writeRegisterData(tpx_address::flags, data);
}

void TMrfTal_RBTopix3::writeLTCData(const TMrfData_Chain2LTC2604& data) const
{
        std::cerr <<"data.getNumBits() "<< data.getNumBits() << std::endl;
        std::cerr <<"data.exportBinString() "<< data.exportBinString() << std::endl;
        writeData(tpx_address::ltcconfig,data);

        //writeRegisterData(tpx_address::ltcconfig, data);
        boardCommand(tpxctrl_value::ltcconfig);

}

void TMrfTal_RBTopix3::startSerializer(const u_int32_t& count) const
{
        write(tpx_address::inputcount, count);
        boardCommand(tpxctrl_value::topix3config);
}


