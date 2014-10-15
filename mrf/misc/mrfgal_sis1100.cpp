/*============================================================*/
/* mrfgal_sis1100.cpp                                         */
/* MVD Readout Framework Generic Access Layer                 */
/* Access Layer for SIS1100 based devices.                    */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrfgal_sis1100.h"
#include "mrfdata.h"

#include <iostream>
#include <iomanip>

// Multithreaded IRQ Handler
/*
void* irqhelper(void* ptr)
{
	static_cast<TMrfGal_Sis1100*>(ptr)->enableIRQ(0x800);
	static_cast<TMrfGal_Sis1100*>(ptr)->waitforIRQFast(0x800);
	static_cast<TMrfGal_Sis1100*>(ptr)->disableIRQ(0x800);
	//static_cast<TMrfGal_Sis1100*>(ptr)->getLastInterrupt();
	std::cerr << "Here I am." << std::endl;
	//sleep(1);
	std::cerr << "Here I was." << std::endl;
}
*/

TMrfGal_Sis1100::TMrfGal_Sis1100()
: TMrfGal()
{
}

// From TMrfGal
mrf::registertype TMrfGal_Sis1100::read(const mrf::addresstype& address) const
{
	if (deviceIsOpen()) {
		sis1100_ctrl_reg reg;
		reg.offset = address;
		reg.error = 0;
		if (doIoctl(SIS1100_CTRL_READ, &reg) < 0) {
			errcode |= mrf_error::read_failed;
			return 0;
		} else if (reg.error) {
			errcode |= mrf_error::read_failed;
			return 0;
		} else {
			errcode = mrf_error::success;
			return reg.val;
		}
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

void TMrfGal_Sis1100::write(const mrf::addresstype& address, const mrf::registertype& value) const
{
	if (deviceIsOpen()) {
		sis1100_ctrl_reg reg;
		reg.offset = address;
		reg.val = value;
		reg.error = 0;
		if (doIoctl(SIS1100_CTRL_WRITE, &reg) < 0) {
			errcode |= mrf_error::write_failed;
		} else if (reg.error) {
			errcode |= mrf_error::write_failed;
		} else {
			errcode = mrf_error::success;
		}
	} else {
		errcode |= mrf_error::device_not_open;
	}
}

u_int32_t TMrfGal_Sis1100::writeData(const mrf::addresstype& address, const TMrfData& data) const
{
	if (deviceIsOpen()) {
		u_int32_t i;
		for (i = 0; i < data.getNumWords(); ++i) {
			write(address, data.getWord(i));
			if (!(lastActionSuccessful())) {
				errcode |= mrf_error::write_data_failed;
				break;
			}
		}
		return i;
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

u_int32_t TMrfGal_Sis1100::readData(const mrf::addresstype& address, TMrfData& data) const
{
	if (deviceIsOpen()) {
		u_int32_t i;
		for (i = 0; i < data.getNumWords(); ++i) {
			data.setWord(i, read(address));
			if (!(lastActionSuccessful())) {
				errcode |= mrf_error::read_data_failed;
				break;
			}
		}
		return i;
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

// In TMrfGal_Sis1100
void TMrfGal_Sis1100::enableIRQ(const u_int32_t& mask) const
{
	if (deviceIsOpen()) {
		sis1100_irq_ctl irqctl;
		irqctl.irq_mask = mask;
		irqctl.signal = sis_magic::irqctlsignal_on;
		doIoctl(SIS1100_IRQ_CTL, &irqctl);
	} else {
		errcode |= mrf_error::device_not_open;
	}
}

void TMrfGal_Sis1100::disableIRQ(const u_int32_t& mask) const
{
	if (deviceIsOpen()) {
		sis1100_irq_ctl irqctl;
		irqctl.irq_mask = mask;
		irqctl.signal = sis_magic::irqctlsignal_off;
		doIoctl(SIS1100_IRQ_CTL, &irqctl);
	} else {
		errcode |= mrf_error::device_not_open;
	}
}

u_int32_t TMrfGal_Sis1100::waitforIRQ(const u_int32_t& mask)
{
	if (deviceIsOpen()) {
		irqget.irq_mask = mask;
		doIoctl(SIS1100_IRQ_WAIT, &irqget);
		irqget.irqs &= mask;
		doIoctl(SIS1100_IRQ_ACK, &irqget.irqs);
		return irqget.irqs;
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

u_int32_t TMrfGal_Sis1100::getIRQ(const u_int32_t& mask)
{
	if (deviceIsOpen()) {
		irqget.irq_mask = mask;
		doIoctl(SIS1100_IRQ_GET, &irqget);
		irqget.irqs &= mask;
		doIoctl(SIS1100_IRQ_ACK, &irqget.irqs);
		return irqget.irqs;
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

u_int32_t TMrfGal_Sis1100::waitforIRQFast(const u_int32_t& mask) const
{
	if (deviceIsOpen()) {
		sis1100_irq_get tmpirq;
		tmpirq.irq_mask = mask;
		doIoctl(SIS1100_IRQ_WAIT, &tmpirq);
		/*
		if (tmpirq.irqs & 2) {
			std::cerr << "Button Pressed: 0x" << std::hex << tmpirq.irqs << std::dec << std::endl;
		}
		if (tmpirq.irqs & !mask) {
			std::cerr << "Additional IRQs: 0x" << std::hex << tmpirq.irqs << std::dec << std::endl;
		}
		*/
		tmpirq.irqs &= mask;
		doIoctl(SIS1100_IRQ_ACK, &tmpirq.irqs);
		return tmpirq.irqs;
	} else {
		return 0;
	}
}

u_int32_t TMrfGal_Sis1100::getIRQFast(const u_int32_t& mask) const
{
	if (deviceIsOpen()) {
		sis1100_irq_get tmpirq;
		tmpirq.irq_mask = mask;
		doIoctl(SIS1100_IRQ_GET, &tmpirq);
		tmpirq.irqs &= mask;
		doIoctl(SIS1100_IRQ_ACK, &tmpirq.irqs);
		return tmpirq.irqs;
	} else {
		return 0;
	}
}

void TMrfGal_Sis1100::ackIRQFast(u_int32_t mask) const
{
	if (deviceIsOpen()) {
		doIoctl(SIS1100_IRQ_ACK, &mask);
	}
}

/*
const mrf::registertype& TMrfGal_Sis1100::getLastDoorbell() const
{
	return doorbell;
}
*/

const u_int32_t& TMrfGal_Sis1100::getLastInterrupt() const
{
	return irqget.irqs;
}

sis1100_ident TMrfGal_Sis1100::getFullIdent() const
{
	sis1100_ident ident;
	ident.local.hw_type = (sis1100_hw_type) 0;
	ident.local.hw_version = 0;
	ident.local.fw_type = 0;
	ident.local.fw_version = 0;
	ident.remote.hw_type = (sis1100_hw_type) 0;
	ident.remote.hw_version = 0;
	ident.remote.fw_type = 0;
	ident.remote.fw_version = 0;
	ident.remote_ok = 0;
	ident.remote_online = 0;
	if (deviceIsOpen()) {
		doIoctl(SIS1100_IDENT, &ident);
	} else {
		errcode |= mrf_error::device_not_open;
	}
	return ident;
}

int TMrfGal_Sis1100::getFullIdentDetail(const sis_ident::ident_detail detail) const
{
	if (deviceIsOpen()) {
		switch (detail) {
		case sis_ident::localhwtype:
			return getFullIdent().local.hw_type;
		case sis_ident::localhwversion:
			return getFullIdent().local.hw_version;
		case sis_ident::localfwtype:
			return getFullIdent().local.fw_type;
		case sis_ident::localfwversion:
			return getFullIdent().local.fw_version;
		case sis_ident::remotehwtype:
			return getFullIdent().remote.hw_type;
		case sis_ident::remotehwversion:
			return getFullIdent().remote.hw_version;
		case sis_ident::remotefwtype:
			return getFullIdent().remote.fw_type;
		case sis_ident::remotefwversion:
			return getFullIdent().remote.fw_version;
		case sis_ident::remoteok:
			return getFullIdent().remote_ok;
		case sis_ident::remoteonline:
			return getFullIdent().remote_online;
		default:
			errcode |= mrf_error::invalid_request;
			return 0;
		}
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

