/*============================================================*/
/* mrftal_rb.cpp                                              */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrftal_rb_sis.h"
#include "mrfdata.h"
#include "mrfdata_dcmconf.h"
#include <iostream>
#include <iomanip>


TMrfTal_RB::TMrfTal_RB()
: TMrfTal(), TMrfGal_Udp(), rb_errcode(0)
{
}

TMrfTal_RB::~TMrfTal_RB()
{
//	if (databuffered) {
//		delete[] buf;
//	}
}

// From TMrfGal
void TMrfTal_RB::openDevice(const char* const devicefile)
{
    TMrfGal_Udp::openDevice(devicefile);
    enableIRQ(rb_irq::inputidle);
    enableIRQ(rb_irq::strobeidle);
}

// From TMrfTal
void TMrfTal_RB::writeRemoteData(const TMrfData& data) const
{
	write(rb_address::bitcountreg, (mrf::registertype) data.getNumBits());
	writeData(rb_address::inputreg, data);
	setFlag(rb_address::control, rb_value::startinput, true);
}

void TMrfTal_RB::readRemoteData(TMrfData& data) const
{
	readOutputBuffer(data, data.getNumWords());
}

// In TMrfTal_RB
mrf::registertype TMrfTal_RB::getFpgaID() const
{
	return read(rb_address::ident);
}

void TMrfTal_RB::pandaReset() const
{
	setFlag(rb_address::control, rb_value::pandareset, true);
}

void TMrfTal_RB::sendStrobe(const u_int32_t& strobelength, const u_int32_t& levelonedelay, const u_int32_t& levelonelength) const
{
	//enableIRQ(0x200);
	ackIRQFast(rb_irq::strobeidle);
	//enableIRQ(0xFFFFFFFF);
	//std::cerr << "Pre IRQ return: " << getIRQFast(0x200) << std::endl;

	write(rb_address::strblen, (mrf::registertype) strobelength);
	write(rb_address::lvl1delay, (mrf::registertype) levelonedelay);
	write(rb_address::lvl1length, (mrf::registertype) levelonelength);
	setFlag(rb_address::control, rb_value::strblvl1, true);

	//std::cerr << "IRQ return: " << waitforIRQFast(0x200) << std::endl;
	waitforIRQFast(rb_irq::strobeidle);
	//disableIRQ(0x200);
	//disableIRQ(0xFFFFFFFF);
}

void TMrfTal_RB::sendLevelOneTrigger(const u_int32_t& levelonelength) const
{
	sendStrobe(0, 0, levelonelength);
}

void TMrfTal_RB::sendSync(const u_int32_t& synclength) const
{
	write(rb_address::synclength, (mrf::registertype) synclength);
	setFlag(rb_address::control, rb_value::startsync, true);
}

void TMrfTal_RB::setLedState(const u_int32_t& led_id, const bool& state) const
{
	if (led_id < 2) {
		setFlag(rb_address::control, sis_led::led[led_id], state);
	} else {
		errcode |= mrf_error::invalid_request;
	}
}

bool TMrfTal_RB::getLedState(const u_int32_t& led_id) const
{
	if (led_id < 2) {
		return getFlag(rb_address::control, sis_led::led[led_id]);
	} else {
		errcode |= mrf_error::invalid_request;
		return false;
	}
}

void TMrfTal_RB::prepareOutputBuffer(const mrf::registertype& datacount) const
{
	if (deviceIsOpen()) {
		setFlag(rb_address::control, rb_value::pandareset, true);
		write(rb_address::bitcountoreg, datacount);
		setFlag(rb_address::control, rb_value::outputenable, true);
	} else {
		rb_errcode |= rb_error::device_not_open;
	}
}

void TMrfTal_RB::prepareEventOutputBuffer(const mrf::registertype& eventcount) const
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

int TMrfTal_RB::readOutputBuffer(TMrfData& data, const u_int32_t& wordcount, const u_int32_t& appendat, const bool& truncate) const
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

bool TMrfTal_RB::isBusy(const mrf::registertype& mask) const
{
	//return static_cast<bool>(read(rb_address::busy));
	return getFlag(rb_address::busy, mask);
}

const u_int32_t& TMrfTal_RB::getLastRBError() const
{
	return rb_errcode;
}

bool TMrfTal_RB::lastRBActionSuccessful() const
{
	return !(static_cast<bool>(rb_errcode));
}

void TMrfTal_RB::configDCM(const int& m, const int& d) const
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

int TMrfTal_RB::initReadout() const
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
int TMrfTal_RB::doReadout(const u_int32_t& wordcount, u_int8_t* const startaddress) const
{
	//if (databuffered) {
	//	delete[] buf;
	//	databuffered = false;
	//}
	if (deviceIsOpen()) {
		//buf = new u_int32_t[wordcount];
		//databuffered = true;
		//data.setNumWords(wordcount);
		//if (startaddress == 0) {
		//	startaddress = reinterpret_cast<u_int8_t*>(&data.regdata[0]);
		//}
		sis1100_vme_block_req req;
		req.size=4;
		req.fifo=1;
		req.num=wordcount;
		req.am=-1;
		req.addr=rb_address::ident;
		//req.data=(unsigned char*)buf;
		//req.data=reinterpret_cast<unsigned u_int8_t*>(&data.regdata[0]);
		//req.data = (startaddress == 0 ? reinterpret_cast<u_int8_t*>(&data.regdata[0]) : startaddress);
		req.data = startaddress;
		req.error=0;
		doIoctl(SIS3100_VME_BLOCK_READ, &req);
		if (!(lastActionSuccessful())) {
			rb_errcode |= rb_error::doreadout_failed;
			//delete[] buf;
			//databuffered = false;
			//std::cerr << "Block Read IOCTL FAIL." << std::endl;
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
			//delete[] buf;
			//databuffered = false;
			return -1;
		}
		return req.num;
	} else {
		rb_errcode |= rb_error::device_not_open;
		return -2;
	}
}

