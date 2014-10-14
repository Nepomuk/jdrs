/*============================================================*/
/* mrftal_rbbase.cpp                                          */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* Basic Functions                                            */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrftal_rbbase.h"
#include "mrfdata.h"
#include "mrfdata_dcmconf.h"
#include "mrfdata_mmcmconf.h"
#include <iostream>
#include <iomanip>



TMrfTal_RBBase::TMrfTal_RBBase()
: TMrfTal(), TMrfGal_Udp(), rb_errcode(0)
{
}

TMrfTal_RBBase::~TMrfTal_RBBase()
{
}

// In TMrfTal_RB
mrf::registertype TMrfTal_RBBase::getFpgaID() const
{
	return read(rb_address::ident);
}

void TMrfTal_RBBase::pandaReset() const
{
	setFlag(rb_address::control, rb_value::pandareset, true);
}

void TMrfTal_RBBase::setLedState(const u_int32_t& led_id, const bool& state) const
{
	if (led_id < 2) {
		setFlag(rb_address::control, sis_led::led[led_id], state);
	} else {
		errcode |= mrf_error::invalid_request;
	}
}

bool TMrfTal_RBBase::getLedState(const u_int32_t& led_id) const
{
	if (led_id < 2) {
		return getFlag(rb_address::control, sis_led::led[led_id]);
	} else {
		errcode |= mrf_error::invalid_request;
		return false;
	}
}

int TMrfTal_RBBase::readOutputBuffer(TMrfData& data, const u_int32_t& wordcount, const u_int32_t& appendat, const bool& truncate) const
{
	if (deviceIsOpen()) {
		int words_read;
		if (data.getNumWords() < appendat + wordcount) {
			data.setNumWords(appendat + wordcount);
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
				data.setNumWords(appendat + words_read);
			}
			//rb_errcode = 0;
			return words_read;
		}
	} else {
		rb_errcode |= rb_error::device_not_open;
		return 0;
	}
}

bool TMrfTal_RBBase::isBusy(const mrf::registertype& mask) const
{
	//return static_cast<bool>(read(rb_address::busy));
	return getFlag(rb_address::busy, mask);
}

const u_int32_t& TMrfTal_RBBase::getLastRBError() const
{
	return rb_errcode;
}

bool TMrfTal_RBBase::lastRBActionSuccessful() const
{
	return !(static_cast<bool>(rb_errcode));
}

void TMrfTal_RBBase::configDCM(const int& m, const int& d) const
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




u_int32_t TMrfTal_RBBase::getLedLine() const
{
	return read(rb_address::ledlinereg);
}

void TMrfTal_RBBase::boardCommand(const mrf::registertype& flag, const bool& status) const
{
	setFlag(rb_address::control, flag, status);
}

void TMrfTal_RBBase::writeRegister(const TMrfDataRegAccess& data, const std::string& item) const
{
	write(data.getAddress(item), data.getValue(item));
}

void TMrfTal_RBBase::writeRegisterData(const mrf::addresstype& address, const TMrfData& data) const
{
        write(address, data.getWord(0));

}

void TMrfTal_RBBase::writeRegisterDataAddress(const TMrfDataAdv1DAddress& data) const
{
	for(std::map<const std::string, TConfItem>::const_iterator iter = data.getConstItemIteratorBegin(); iter != data.getConstItemIteratorEnd(); ++iter)
	{
		write(data.getLocalItemAddress(iter->first),data.getLocalItemValue(iter->first));
	}

}

int TMrfTal_RBBase::initReadout() const
{
	if (deviceIsOpen()) {
		int len[2]={1, 1};
		doIoctl(SIS1100_MINDMALEN, len);
		if (lastActionSuccessful()) {
			rb_errcode = 0;
			return 0;
		} else {
			rb_errcode |= rb_error::initreadout_failed;
			return -1;
		}
	} else {
		rb_errcode |= rb_error::device_not_open;
		return -2;
	}
}


//const int TMrfTal_RB::doReadout(TMrfData& data, const u_int32_t& wordcount, u_int8_t* const startaddress)
int TMrfTal_RBBase::doReadout(const u_int32_t& wordcount, u_int8_t* const startaddress) const
{
	if (deviceIsOpen()) {
		sis1100_vme_block_req req;
		req.size=4;
		req.fifo=1;
		req.num=wordcount;
		req.am=-1;
		req.addr=rb_address::ident;
		req.data = startaddress;
		req.error=0;
		doIoctl(SIS3100_VME_BLOCK_READ, &req);
		if (!(lastActionSuccessful())) {
			rb_errcode |= rb_error::doreadout_failed;
			return -1;
		}
		switch (req.error) {
		case sis_magic::nomoredata:	/* normal case, more data requested than available */
			rb_errcode = 0;
			break;
		case sis_magic::success:	/* not enough data requested */
			rb_errcode |= rb_error::more_data_available;
			break;
		default:
			rb_errcode |= rb_error::doreadout_failed;
			std::cerr << "Block Read FAIL: " << std::hex << req.error << std::dec << std::endl;
			return -1;

		}
		return req.num;
	} else {
		rb_errcode |= rb_error::device_not_open;
		return -2;
	}
}

