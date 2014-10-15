/*============================================================*/
/* mrfcal_afei3.cpp                                           */
/* MVD Readout Framework Chip Access Layer                    */
/* Chip Access Layer for Atlas Frontend I3                    */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrfcal_afei3.h"
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <time.h>

static const unsigned long idletimer = 0;

TMrfCal_AFEI3::TMrfCal_AFEI3()
: TMrfCal(), TMrfTal_RB()
{
}

void TMrfCal_AFEI3::writeToChip(const TMrfData& data) const
{
	//usleep(idletimer);
	//if (isBusy(rb_value::startinput)) {
		//std::cerr << "Input Busy Conflict." << std::endl;
		//u_int32_t waitedfor = 0;
	//	usleep(idletimer);
	//	while (isBusy(rb_value::startinput)) {
	//		usleep(idletimer);
			//++waitedfor;;
	//	}
		//std::cerr << "Input Busy Conflict: " << waitedfor << " us" << std::endl;
	//}

	//spawn sync process
	//pthread_t syncprocess;
	//TMrfGal_Sis1100* ptr = (TMrfGal_Sis1100*) this;
	//int threaderr = pthread_create(&syncprocess, NULL, irqhelper, (void*) ptr);

	//enableIRQ(0x802);
	ackIRQFast(rb_irq::inputidle);
	//async data write
	writeRemoteData(data);
	waitforIRQFast(rb_irq::inputidle);
	//disableIRQ(0x800);
	//join sync process
	//pthread_join(syncprocess, NULL);
}

void TMrfCal_AFEI3::readFromChip(TMrfData& data)
{
	//clock_t start = clock();
	readRemoteData(data);
	//std::cerr << "Data read time: " << (double(clock()-start)*1000)/CLOCKS_PER_SEC << std::endl;
}

void TMrfCal_AFEI3::configureGlobReg(const TMrfData_AFEI3& data) const
{
	if (deviceIsOpen()) {
		TMrfData_AFEI3 tmp = data;
		tmp.setDefaultAddress(data.getCommandRegValue("Address"));
		tmp.setDefaultBroadcast(data.getCommandRegValue("Broadcast"));
		tmp.setCommandRegValue("ClockGlobal", 1);
		tmp.assembleCommandReg();
		tmp.assembleGlobReg();
		writeToChip(tmp);
		tmp.clearDataStream();
		tmp.clearCommandReg();
		tmp.setCommandRegValue("WriteGlobal", 3);
		tmp.assembleCommandReg();
		writeToChip(tmp);
		tmp.clearDataStream();
		tmp.clearCommandReg();
		tmp.assembleCommandReg();
		writeToChip(tmp);
	} else {
		errcode |= mrf_error::device_not_open;
	}
}

void TMrfCal_AFEI3::configurePixReg(const TMrfData_AFEI3& data) const
{
	if (deviceIsOpen()) {
		data.updatePixRegPosCache();
		TMrfData_AFEI3 tmp = data;
		tmp.setDefaultAddress(data.getCommandRegValue("Address"));
		tmp.setDefaultBroadcast(data.getCommandRegValue("Broadcast"));
		//clock_t start = clock();
		for (std::map<const std::string, TConfItem>::const_iterator iter = tmp.getPixRegIteratorBegin(0, 0); iter != tmp.getPixRegIteratorEnd(0, 0); ++iter) {
			//start = clock();
			tmp.clearDataStream();
			tmp.clearCommandReg();
			tmp.setCommandRegValue("ClockPixel", 1);
			tmp.assembleCommandReg();
			tmp.assemblePixReg(iter->first);
			//std::cerr << "Value assembly time: " << (double(clock()-start)*1000)/CLOCKS_PER_SEC << std::endl;
			//start = clock();
			writeToChip(tmp);
			//std::cerr << "Pixreg write time: " << (double(clock()-start)*1000)/CLOCKS_PER_SEC << std::endl;
			tmp.clearDataStream();
			tmp.clearCommandReg();
			tmp.setCommandRegValue(iter->first, 1);
			tmp.assembleCommandReg();
			writeToChip(tmp);
			tmp.clearDataStream();
			tmp.clearCommandReg();
			tmp.assembleCommandReg();
			writeToChip(tmp);
			//std::cerr << "Value config time: " << (double(clock()-start)*1000)/CLOCKS_PER_SEC << std::endl;
		}
	} else {
		errcode |= mrf_error::device_not_open;
	}
}

void TMrfCal_AFEI3::shiftIntoPixReg(const TMrfData_AFEI3& data) const
{
	TMrfData_AFEI3 tmp = data;
	tmp.setDefaultAddress(data.getCommandRegValue("Address"));
	tmp.setDefaultBroadcast(data.getCommandRegValue("Broadcast"));
	tmp.clearCommandReg();
	tmp.setCommandRegValue("ClockPixel", 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::shiftItemIntoPixReg(const TMrfData_AFEI3& data, const std::string& item) const
{
	data.updatePixRegPosCache();
	//TMrfData_AFEI3 tmp = data;
	TMrfData_AFEI3 tmp;
	tmp.setDefaultAddress(data.getCommandRegValue("Address"));
	tmp.setDefaultBroadcast(data.getCommandRegValue("Broadcast"));
	tmp.clearCommandReg();
	tmp.setCommandRegValue("ClockPixel", 1);
	tmp.assembleCommandReg();
	tmp.assemblePixReg(item, data);
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::latchPixReg(const mrf::registertype& address, const mrf::registertype& broadcast, const std::string& item) const
{
	TMrfData_AFEI3 tmp;
	tmp.setDefaultAddress(address);
	tmp.setDefaultBroadcast(broadcast);
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue(item, 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::readPixReg(const mrf::registertype& address, const mrf::registertype& broadcast, const std::string& item) const
{
	TMrfData_AFEI3 tmp;
	//readbit an
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.setCommandRegValue("ReadPixel", 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//zusätzlich latchauswahl
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.setCommandRegValue("ReadPixel", 1);
	tmp.setCommandRegValue(item, 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//zusätzlich clock und 2 takte
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.setCommandRegValue("ReadPixel", 1);
	tmp.setCommandRegValue(item, 1);
	tmp.setCommandRegValue("ClockPixel", 1);
	tmp.assembleCommandReg();
	tmp.appendBit(false);
	tmp.appendBit(false);
	writeToChip(tmp);
		//clock wieder aus, nur noch latchauswahl und read an
		//tmp.clearDataStream();
		//tmp.clearCommandReg();
		//tmp.setCommandRegValue("Address", address);
		//tmp.setCommandRegValue("Broadcast", broadcast);
		//tmp.setCommandRegValue("ReadPixel", 1);
		//tmp.setCommandRegValue(item, 1);
		//tmp.assembleCommandReg();
		//writeToChip(tmp);
	//clock und latchauswahl wieder aus, nur noch read an
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.setCommandRegValue("ReadPixel", 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::insertPixRegData(const TMrfData_AFEI3& data, const std::string& item) const
{
	shiftIntoPixReg(data);
	latchPixReg(data.getCommandRegValue("Address"), data.getCommandRegValue("Broadcast"), item);
}

void TMrfCal_AFEI3::configurePixRegItem(const TMrfData_AFEI3& data, const std::string& item) const
{
	if (item == mrf_afei3::fdac) {
		configurePixRegItem(data, item + "0");
		configurePixRegItem(data, item + "1");
		configurePixRegItem(data, item + "2");
	} else if (item == mrf_afei3::tdac) {
		configurePixRegItem(data, item + "0");
		configurePixRegItem(data, item + "1");
		configurePixRegItem(data, item + "2");
		configurePixRegItem(data, item + "3");
		configurePixRegItem(data, item + "4");
		configurePixRegItem(data, item + "5");
		configurePixRegItem(data, item + "6");
	} else {
		shiftItemIntoPixReg(data, item);
		latchPixReg(data.getCommandRegValue("Address"), data.getCommandRegValue("Broadcast"), item);
	}
}

void TMrfCal_AFEI3::shiftIntoGlobReg(const TMrfData_AFEI3& data) const
{
	TMrfData_AFEI3 tmp;
	tmp.importBinString(data.exportBinString());
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", data.getCommandRegValue("Address"));
	tmp.setCommandRegValue("Broadcast", data.getCommandRegValue("Broadcast"));
	tmp.setCommandRegValue("ClockGlobal", 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", data.getCommandRegValue("Address"));
	tmp.setCommandRegValue("Broadcast", data.getCommandRegValue("Broadcast"));
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::latchGlobReg(const mrf::registertype& address, const mrf::registertype& broadcast) const
{
	TMrfData_AFEI3 tmp;
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.setCommandRegValue("WriteGlobal", 3);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::readGlobReg(const mrf::registertype& address, const mrf::registertype& broadcast) const
{
	TMrfData_AFEI3 tmp;
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.setCommandRegValue("ReadGlobal", 1);
	tmp.assembleCommandReg();
	writeToChip(tmp);
	//clear command
	tmp.clearDataStream();
	tmp.clearCommandReg();
	tmp.setCommandRegValue("Address", address);
	tmp.setCommandRegValue("Broadcast", broadcast);
	tmp.assembleCommandReg();
	writeToChip(tmp);
}

void TMrfCal_AFEI3::configureFE(const TMrfData_AFEI3& data) const
{
	configureGlobReg(data);
	configurePixReg(data);
}



