/*============================================================*/
/* mrftal_rbudp.cpp                                           */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via UDP Ethernet Link                            */
/* Basic Functions                                            */
/*                                                 A. Goerres */
/*============================================================*/


#include "mrftal_rbudp.h"
#include "mrfdata.h"
#include "mrfdata_dcmconf.h"
#include "mrfdata_mmcmconf.h"
#include <iostream>
#include <iomanip>



TMrfTal_RBUDP::TMrfTal_RBUDP()
: TMrfTal(), TMrfGal_Udp(), rb_errcode(0)
{
}

TMrfTal_RBUDP::~TMrfTal_RBUDP()
{
}

// In TMrfTal_RB
mrf::registertype TMrfTal_RBUDP::getFpgaID() const
{
	return read(rb_address::ident);
}

void TMrfTal_RBUDP::pandaReset() const
{
	setFlag(rb_address::control, rb_value::pandareset, true);
}

void TMrfTal_RBUDP::setLedState(const u_int32_t& led_id, const bool& state) const
{
	if (led_id < 2) {
		setFlag(rb_address::control, sis_led::led[led_id], state);
	} else {
		errcode |= mrf_error::invalid_request;
	}
}

bool TMrfTal_RBUDP::getLedState(const u_int32_t& led_id) const
{
	if (led_id < 2) {
		return getFlag(rb_address::control, sis_led::led[led_id]);
	} else {
		errcode |= mrf_error::invalid_request;
		return false;
	}
}

void TMrfTal_RBUDP::prepareOutputBuffer(const mrf::registertype& datacount) const
{
    if (deviceIsOpen()) {
        setFlag(rb_address::control, rb_value::pandareset, true);
        write(rb_address::bitcountoreg, datacount);
        setFlag(rb_address::control, rb_value::outputenable, true);
    } else {
        rb_errcode |= rb_error::device_not_open;
    }
}

void TMrfTal_RBUDP::prepareEventOutputBuffer(const mrf::registertype& eventcount) const
{
    if (deviceIsOpen()) {
        setFlag(rb_address::control, rb_value::pandareset, true);
        write(rb_address::bitcountoreg, eventcount);
        setFlag(rb_address::control, rb_value::eoedetect, true);
        setFlag(rb_address::control, rb_value::outputenable, true);
    } else {
        rb_errcode |= rb_error::device_not_open;
    }
}

int TMrfTal_RBUDP::readOutputBuffer(TMrfData& data, const u_int32_t& wordcount, const u_int32_t& appendat, const bool& truncate) const
{
    if (deviceIsOpen()) {
        int words_read;
        if (data.getNumWords() < appendat + wordcount /*+ 1*/) {
            data.setNumWords(appendat + wordcount /*+ 1*/);
        }
        if (initReadout() < 0) {
            if (truncate) {
                data.clearDataStream();
            }
            rb_errcode |= rb_error::read_failed;
            return 0;
        } else if ((words_read = doReadout(wordcount, reinterpret_cast<u_int8_t*>(&data.regdata[appendat]))) < 0) {
            if (truncate) {
                data.clearDataStream();
            }
            rb_errcode |= rb_error::read_failed;
            return 0;
        } else {
            if (truncate) {
                data.setNumWords(appendat + words_read /*+ 1*/);
            }
            //rb_errcode = 0;
            return words_read;
        }
    } else {
        rb_errcode |= rb_error::device_not_open;
        return 0;
    }
}

bool TMrfTal_RBUDP::isBusy(const mrf::registertype& mask) const
{
	//return static_cast<bool>(read(rb_address::busy));
	return getFlag(rb_address::busy, mask);
}

const u_int32_t& TMrfTal_RBUDP::getLastRBError() const
{
	return rb_errcode;
}

bool TMrfTal_RBUDP::lastRBActionSuccessful() const
{
	return !(static_cast<bool>(rb_errcode));
}

void TMrfTal_RBUDP::configDCM(const int& m, const int& d) const
{
	if (deviceIsOpen()) {
		TMrfData_DCMConf dcmconf;
		dcmconf.setItemValue("Multiplier", "Value", m);
		dcmconf.setItemValue("Divider", "Value", d);
		dcmconf.assemble("Multiplier");
		writeData(rb_address::dcmconfigreg, dcmconf);
		setFlag(rb_address::control, rb_value::configdcm, true);
		dcmconf.assemble("Divider");
		writeData(rb_address::dcmconfigreg, dcmconf);
		setFlag(rb_address::control, rb_value::configdcm, true);
	} else {
		errcode |= mrf_error::device_not_open;
	}
}




u_int32_t TMrfTal_RBUDP::getLedLine() const
{
	return read(rb_address::ledlinereg);
}

void TMrfTal_RBUDP::boardCommand(const mrf::registertype& flag, const bool& status) const
{
	setFlag(rb_address::control, flag, status);
}

void TMrfTal_RBUDP::writeRegister(const TMrfDataRegAccess& data, const std::string& item) const
{
	write(data.getAddress(item), data.getValue(item));
}

void TMrfTal_RBUDP::writeRegisterData(const mrf::addresstype& address, const TMrfData& data) const
{
    write(address, data.getWord(0));
}

void TMrfTal_RBUDP::writeRegisterDataAddress(const TMrfDataAdv1DAddress& data) const
{
	for(std::map<const std::string, TConfItem>::const_iterator iter = data.getConstItemIteratorBegin(); iter != data.getConstItemIteratorEnd(); ++iter)
	{
		write(data.getLocalItemAddress(iter->first),data.getLocalItemValue(iter->first));
	}

}

int TMrfTal_RBUDP::initReadout() const
{
    if (deviceIsOpen()) {
        // Do we have to actually do something here?
        // I don't know.
        /*if ( deviceIsOnline() ) {
            rb_errcode = 0;
            return 0;
        } else {
            rb_errcode |= rb_error::initreadout_failed;
            return -1;
        }*/
    } else {
        rb_errcode |= rb_error::device_not_open;
        return -2;
    }
}


//const int TMrfTal_RB::doReadout(TMrfData& data, const u_int32_t& wordcount, u_int8_t* const startaddress)
int TMrfTal_RBUDP::doReadout(const u_int32_t& wordcount, u_int8_t* const startaddress) const
{
    if (deviceIsOpen()) {
        udpRequest req;
        req.dataFlag = udpDataFlag::readDMA;
        req.registerAddress = rb_address::ident;
        req.memoryAddress = (char*)startaddress;
        req.wordcount = wordcount;

        // deal with the UDP request
        doIoctl(req.dataFlag, &req);

        if ( !lastActionSuccessful() ) {
            rb_errcode |= rb_error::doreadout_failed;
            return -1;
        }

        // If the DMA buffer is empty, 0xeeeeeeee is returned. Check for that...
        if ( *(u_int32_t*)(req.memoryAddress) == 0xeeeeeeee && req.wordcount == 1 )
            req.wordcount = 0;

        return req.wordcount;
    } else {
        rb_errcode |= rb_error::device_not_open;
        return -2;
    }
}


// From TMrfTal
void TMrfTal_RBUDP::writeRemoteData(const TMrfData& data) const
{
    write(rb_address::bitcountreg, (mrf::registertype) data.getNumBits());
    writeData(rb_address::inputreg, data);
    setFlag(rb_address::control, rb_value::startinput, true);
}

void TMrfTal_RBUDP::readRemoteData(TMrfData& data) const
{
    readOutputBuffer(data, data.getNumWords());
}


const double& TMrfTal_RBUDP::configMMCM(const double& freq)
{

    if(deviceIsOpen()){
        _mmcmconf.setFrequency(freq);
        writeRegisterDataAddress(_mmcmconf);
        write(mmcm_address::mmcmconfigregister,00000000000000000000000000000010);
        return _mmcmconf.getFrequency();
    } else {
        errcode |= mrf_error::device_not_open;
        _mmcmconf.setFrequency(0);
        return _mmcmconf.getFrequency();
    }
}
