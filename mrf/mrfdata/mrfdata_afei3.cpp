/*============================================================*/
/* mrfdata_afei3.cpp                                          */
/* MVD Readout Framework Data Storage                         */
/* Specific to Atlas FE-I3 Registers                          */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrfdata_afei3.h"
#include "mrf_confitem.h"
#include <map>
#include "mrftools.h"

using mrftools::getIntBit;
using mrftools::setIntBit;
using mrftools::shiftBy;

TMrfData_AFEI3::TMrfData_AFEI3(const mrf::registertype& address, const mrf::registertype& broadcast)
: TMrfData(), feaddress(address), febroadcast(broadcast)
{
	initMaps();
}

void TMrfData_AFEI3::initCommandReg()
{
	cmdreg.clear();
	cmdreg["Address"] = TConfItem(0, 0, 4);
	cmdreg["Broadcast"] = TConfItem(0, 4, 1);
	cmdreg["CommandParity"] = TConfItem(0, 0+5, 1);
	cmdreg["RefReset"] = TConfItem(0, 1+5, 1);
	cmdreg["SoftReset"] = TConfItem(0, 2+5, 2);
	cmdreg["ClockGlobal"] = TConfItem(0, 4+5, 1);
	cmdreg["WriteGlobal"] = TConfItem(0, 5+5, 2);
	cmdreg["ReadGlobal"] = TConfItem(0, 7+5, 1);
	cmdreg["ClockPixel"] = TConfItem(0, 8+5, 1);
	cmdreg["Hitbus"] = TConfItem(0, 9+5, 1);
	cmdreg["Select"] = TConfItem(0, 10+5, 1);
	cmdreg["Mask"] = TConfItem(0, 11+5, 1);
	cmdreg[mrf_afei3::tdac + "0"] = TConfItem(0, 12+5, 1);
	cmdreg[mrf_afei3::tdac + "1"] = TConfItem(0, 13+5, 1);
	cmdreg[mrf_afei3::tdac + "2"] = TConfItem(0, 14+5, 1);
	cmdreg[mrf_afei3::tdac + "3"] = TConfItem(0, 15+5, 1);
	cmdreg[mrf_afei3::tdac + "4"] = TConfItem(0, 16+5, 1);
	cmdreg[mrf_afei3::tdac + "5"] = TConfItem(0, 17+5, 1);
	cmdreg[mrf_afei3::tdac + "6"] = TConfItem(0, 18+5, 1);
	cmdreg[mrf_afei3::fdac + "0"] = TConfItem(0, 19+5, 1);
	cmdreg[mrf_afei3::fdac + "1"] = TConfItem(0, 20+5, 1);
	cmdreg[mrf_afei3::fdac + "2"] = TConfItem(0, 21+5, 1);
	cmdreg["Kill"] = TConfItem(0, 22+5, 1);
	cmdreg["ReadPixel"] = TConfItem(0, 23+5, 1);
	//cmdreg["Address"] = TConfItem(0, 24, 4);
	//cmdreg["Broadcast"] = TConfItem(0, 28, 1);
}

void TMrfData_AFEI3::initGlobReg()
{
	globreg.clear();
	globreg["GlobalParity"] = TConfItem(0, 0, 1);
	globreg["Latency"] = TConfItem(0, 1, 8);
	globreg["SelfTriggerDelay"] = TConfItem(0, 9, 4);
	globreg["SelfTriggerWidth"] = TConfItem(0, 13, 4);
	globreg["EnableSelfTrigger"] = TConfItem(0, 17, 1);
	globreg["EnableHitParity"] = TConfItem(0, 18, 1);
	globreg["SelectDO"] = TConfItem(0, 19, 4);
	globreg["SelectMonHit"] = TConfItem(0, 23, 4);
	globreg["TSITSCEnable"] = TConfItem(0, 27, 1);
	globreg["SelectDataPhase"] = TConfItem(0, 28, 1);
	globreg["EnableEOEParity"] = TConfItem(0, 29, 1);
	globreg["HitBusScaler"] = TConfItem(0, 30, 8);
	globreg["MonLeakADCRefTest"] = TConfItem(0, 38, 1);
	globreg["MonLeakADCDAC"] = TConfItem(0, 39, 10);
	globreg["MonLeakADCDACTest"] = TConfItem(0, 49, 1);
	globreg["MonLeakADCEnableComparator"] = TConfItem(0, 50, 1);
	globreg["MonLeakADCMonComp"] = TConfItem(0, 51, 1); //read only
	globreg["ARegTrim"] = TConfItem(0, 52, 2);
	globreg["EnableARegMeas"] = TConfItem(0, 54, 1);
	globreg["ARegMeas"] = TConfItem(0, 55, 2);
	globreg["EnableAReg"] = TConfItem(0, 57, 1);
	globreg["EnableLVDSReferenceMeas"] = TConfItem(0, 58, 1);
	globreg["DRegTrim"] = TConfItem(0, 59, 2);
	globreg["EnableDRegMeas"] = TConfItem(0, 61, 1);
	globreg["DRegMeas"] = TConfItem(0, 62, 2);
	globreg["CapMeasureDevice"] = TConfItem(0, 64, 4);
	globreg["CapMeasureClock"] = TConfItem(0, 68, 2);
	globreg["EnableCapTest"] = TConfItem(0, 70, 1);
	globreg["EnableAnalogOut"] = TConfItem(0, 71, 1);
	globreg["TestPixelMUX"] = TConfItem(0, 72, 2);
	globreg["EnableVCalMeas"] = TConfItem(0, 74, 1);
	globreg["EnableLeakMeas"] = TConfItem(0, 75, 1);
	globreg["EnableBufferBoost"] = TConfItem(0, 76, 1);
	globreg["EnableCol8"] = TConfItem(0, 77, 1);
	globreg["TestDACforIVDD2DAC"] = TConfItem(0, 78, 1);
	globreg["IVDD2DAC"] = TConfItem(0, 79, 8);
	globreg["IDDAC"] = TConfItem(0, 87, 8);
	globreg["TestDACforIDDAC"] = TConfItem(0, 95, 1);
	globreg["EnableCol7"] = TConfItem(0, 96, 1);
	globreg["TestDACforIP2DAC"] = TConfItem(0, 97, 1);
	globreg["IP2DAC"] = TConfItem(0, 98, 8);
	globreg["IPDAC"] = TConfItem(0, 106, 8);
	globreg["TestDACforIPDAC"] = TConfItem(0, 114, 1);
	globreg["EnableCol6"] = TConfItem(0, 115, 1);
	globreg["TestDACforITrimThDAC"] = TConfItem(0, 116, 1);
	globreg["ITrimThDAC"] = TConfItem(0, 117, 8);
	globreg["IFDAC"] = TConfItem(0, 125, 8);
	globreg["TestDACforIFDAC"] = TConfItem(0, 133, 1);
	globreg["EnableCol5"] = TConfItem(0, 134, 1);
	globreg["TestDACforITrimIFDAC"] = TConfItem(0, 135, 1);
	globreg["ITrimIFDAC"] = TConfItem(0, 136, 8);
	globreg["VCalDAC"] = TConfItem(0, 144, 10);
	globreg["TestDACforVCalDAC"] = TConfItem(0, 154, 1);
	globreg["EnableCol4"] = TConfItem(0, 155, 1);
	globreg["HighInjectionCapSelection"] = TConfItem(0, 156, 1);
	globreg["EnableExtInj"] = TConfItem(0, 157, 1);
	globreg["TestAnalogReference"] = TConfItem(0, 158, 1);
	globreg["EOCMUXControl"] = TConfItem(0, 159, 2);
	globreg["CEUClockControl"] = TConfItem(0, 161, 2);
	globreg["EnableDigitalInjection"] = TConfItem(0, 163, 1);
	globreg["EnableCol3"] = TConfItem(0, 164, 1);
	globreg["TestDACforITH1DAC"] = TConfItem(0, 165, 1);
	globreg["ITH1DAC"] = TConfItem(0, 166, 8);
	globreg["ITH2DAC"] = TConfItem(0, 174, 8);
	globreg["TestDACforITH2DAC"] = TConfItem(0, 182, 1);
	globreg["EnableCol2"] = TConfItem(0, 183, 1);
	globreg["TestDACforILDAC"] = TConfItem(0, 184, 1);
	globreg["ILDAC"] = TConfItem(0, 185, 8);
	globreg["IL2DAC"] = TConfItem(0, 193, 8);
	globreg["TestDACforIL2DAC"] = TConfItem(0, 201, 1);
	globreg["EnableCol1"] = TConfItem(0, 202, 1);
	globreg["ThrMin"] = TConfItem(0, 203, 8);
	globreg["ThrDub"] = TConfItem(0, 211, 8);
	globreg["ReadMode"] = TConfItem(0, 219, 2);
	globreg["EnableCol0"] = TConfItem(0, 221, 1);
	globreg["HitBusEnable"] = TConfItem(0, 222, 1);
	globreg["GlobalTDAC"] = TConfItem(0, 223, 5);
	globreg["EnableTune"] = TConfItem(0, 228, 1);
	globreg["EnableBiasCompensation"] = TConfItem(0, 229, 1);
	globreg["EnableIPMonitor"] = TConfItem(0, 230, 1);
}

void TMrfData_AFEI3::initPixReg()
{
	u_int32_t col, row;
	for (col = 0; col < mrf_afei3_reglength::logpixcols; ++col) {
		for (row = 0; row < mrf_afei3_reglength::logpixrows; ++row) {
			pixreg[col][row].clear();
			pixreg[col][row]["Hitbus"] = TConfItem(0, 0, 1);
			pixreg[col][row]["Select"] = TConfItem(0, 1, 1);
			pixreg[col][row]["Mask"] = TConfItem(0, 2, 1);
			pixreg[col][row][mrf_afei3::tdac + "0"] = TConfItem(0, 3, 1);
			pixreg[col][row][mrf_afei3::tdac + "1"] = TConfItem(0, 4, 1);
			pixreg[col][row][mrf_afei3::tdac + "2"] = TConfItem(0, 5, 1);
			pixreg[col][row][mrf_afei3::tdac + "3"] = TConfItem(0, 6, 1);
			pixreg[col][row][mrf_afei3::tdac + "4"] = TConfItem(0, 7, 1);
			pixreg[col][row][mrf_afei3::tdac + "5"] = TConfItem(0, 8, 1);
			pixreg[col][row][mrf_afei3::tdac + "6"] = TConfItem(0, 9, 1);
			pixreg[col][row][mrf_afei3::fdac + "0"] = TConfItem(0, 10, 1);
			pixreg[col][row][mrf_afei3::fdac + "1"] = TConfItem(0, 11, 1);
			pixreg[col][row][mrf_afei3::fdac + "2"] = TConfItem(0, 12, 1);
			pixreg[col][row]["Kill"] = TConfItem(0, 13, 1);
		}
	}
}

void TMrfData_AFEI3::clearCommandReg(const bool& clearpersistent)
{
	std::map<const std::string, TConfItem>::iterator iter;
	for (iter = cmdreg.begin(); iter != cmdreg.end(); ++iter) {
		iter->second.value = 0;
	}
	if (!clearpersistent) {
		setCommandRegValue("Address", feaddress);
		setCommandRegValue("Broadcast", febroadcast);
	}
}

void TMrfData_AFEI3::clearGlobReg()
{
	std::map<const std::string, TConfItem>::iterator iter;
	for (iter = globreg.begin(); iter != globreg.end(); ++iter) {
		iter->second.value = 0;
	}
}

void TMrfData_AFEI3::clearPixReg()
{
	u_int32_t col, row;
	std::map<const std::string, TConfItem>::iterator iter;
	for (col = 0; col < getNumPixCols(); ++col) {
		for (row = 0; row < getNumPixRows(); ++row) {
			for (iter = pixreg[col][row].begin(); iter != pixreg[col][row].end(); ++iter) {
				iter->second.value = 0;
			}
		}
	}
}

/*
void TMrfData_AFEI3::assemble(const mrf_afei3::assemblymode mode)
{
	if (mode == mrf_afei3::command) {
		setNumBits(mrf_afei3_reglength::command);
		assembleCommandReg();
	} else if (mode == mrf_afei3::global) {
		setNumBits(mrf_afei3_reglength::command + mrf_afei3_reglength::global);
		assembleCommandReg();
		assembleGlobReg();
	} else if (mode == mrf_afei3::pixel) {
		setNumBits(mrf_afei3_reglength::command + mrf_afei3_reglength::pixrows);
		assembleCommandReg();
		//assemblePixReg(getActivePixColsFromGlobReg(), getWritePixItemFromCmdReg());
	}
}
*/

void TMrfData_AFEI3::disassemble()
{
}

void TMrfData_AFEI3::disassembleCommandReg()
{
// TODO: l�ngentest!!!
	std::map<const std::string, TConfItem>::iterator iter;
	for (iter = cmdreg.begin(); iter != cmdreg.end(); ++iter) {
		setCommandRegValue(iter->first, getBitBlock(mrf_afei3_reglength::command - iter->second.position - iter->second.length, iter->second.length, 0, true));
	}
}

void TMrfData_AFEI3::disassembleGlobReg()
{
// TODO: l�ngentest!!!
	std::map<const std::string, TConfItem>::iterator iter;
	for (iter = globreg.begin(); iter != globreg.end(); ++iter) {
		setGlobRegValue(iter->first, getBitBlock(mrf_afei3_reglength::global - iter->second.position - iter->second.length, iter->second.length, 0, true));
	}
}

void TMrfData_AFEI3::disassemblePixReg(const u_int32_t& activecolumns, const std::string& item)
{
// TODO: l�ngentest!!!
	u_int32_t column = 0;
	while (!(getIntBit(column, activecolumns)) && (column < (getNumPixCols() - 1))) {
		++column;
	}
	u_int32_t i;
	for (i = 0; i < getNumBits(); ++i) {
		setPixRegValue(item, column, i % getNumPixRows(), getBit(i));
		if (((i + 1) % getNumPixRows()) == 0) {
			++column;
			while (!(getIntBit(column, activecolumns)) && column < (getNumPixCols() - 1)) {
				++column;
			}
		}
	}
}

void TMrfData_AFEI3::assembleCommandReg()
{
	u_int32_t registerlength = mrf_afei3_reglength::command;
	if (getNumBits() < registerlength) {
		setNumBits(registerlength);
	}
	std::map<const std::string, TConfItem>::const_iterator iter;
	for (iter = getCommandRegIteratorBegin(); iter != getCommandRegIteratorEnd(); ++iter) {
		setBitBlock(mrf_afei3_reglength::command - iter->second.position - iter->second.length, iter->second.length, iter->second.value, 0, true);
	}
}

void TMrfData_AFEI3::assembleGlobReg()
{
	u_int32_t registerlength = mrf_afei3_reglength::command + mrf_afei3_reglength::global;
	if (getNumBits() < registerlength) {
		setNumBits(registerlength);
	}
	std::map<const std::string, TConfItem>::const_iterator iter;
	for (iter = getGlobRegIteratorBegin(); iter != getGlobRegIteratorEnd(); ++iter) {
		setBitBlock(mrf_afei3_reglength::global - iter->second.position - iter->second.length + mrf_afei3::globregoffset, iter->second.length, iter->second.value, 0, true);
	}
}

void TMrfData_AFEI3::assemblePixReg(const std::string& item)
{
	setNumBits((getNumPixRows() * getActivePixColCount()) + mrf_afei3_reglength::command);
	updatePixRegPosCache();
	for (u_int32_t i = 0; i < getNumPixCols(); ++i) {
		if (getPixColActivated(i)) {
			for (u_int32_t j = 0; j < getNumPixRows(); ++j) {
				setBit(mrf_afei3::pixregoffset + getPixRegPositionFromCoordsFast(i, j), pixreg[i][j].find(item)->second.value);
			}
		}
	}
}


void TMrfData_AFEI3::assemblePixReg(const std::string& item, const TMrfData_AFEI3& rhs)
{
	//u_int32_t registerlength = mrf_afei3_reglength::command;
	//u_int32_t colcount = 0;
	//u_int32_t i, j;
	//registerlength += getNumPixRows() * getActivePixColCount();
	//setNumBits(registerlength);
	setNumBits((getNumPixRows() * rhs.getActivePixColCount()) + mrf_afei3_reglength::command);
	rhs.updatePixRegPosCache();
	for (u_int32_t i = 0; i < getNumPixCols(); ++i) {
		if (rhs.getPixColActivated(i)) {
			//registerlength += getNumPixRows();
			//if (getNumBits() < registerlength) {
			//	setNumBits(registerlength);
			//}
			for (u_int32_t j = 0; j < getNumPixRows(); ++j) {
				//setBit(mrf_afei3::pixregoffset + getPixRegPositionFromCoords(i, j), pixreg[i][j].find(item)->second.value);
				setBit(mrf_afei3::pixregoffset + rhs.getPixRegPositionFromCoordsFast(i, j), rhs.pixreg[i][j].find(item)->second.value);
			}
			//++colcount;
		}
	}
}

void TMrfData_AFEI3::assembleCommandRegValue(const std::string& item)
{
	setBitBlock(mrf_afei3_reglength::command - cmdreg.find(item)->second.position - cmdreg.find(item)->second.length, cmdreg.find(item)->second.length, cmdreg.find(item)->second.value, 0, true);
}

void TMrfData_AFEI3::assembleGlobRegValue(const std::string& item)
{
	setBitBlock(mrf_afei3_reglength::global - globreg.find(item)->second.position - globreg.find(item)->second.length + mrf_afei3::globregoffset, globreg.find(item)->second.length, globreg.find(item)->second.value, 0, true);
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3::getCommandRegIteratorBegin() const
{
	return cmdreg.begin();
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3::getGlobRegIteratorBegin() const
{
	return globreg.begin();
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3::getPixRegIteratorBegin(const u_int32_t& i, const u_int32_t& j) const
{
	return pixreg[i][j].begin();
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3::getCommandRegIteratorEnd() const
{
	return cmdreg.end();
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3::getGlobRegIteratorEnd() const
{
	return globreg.end();
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3::getPixRegIteratorEnd(const u_int32_t& i, const u_int32_t& j) const
{
	return pixreg[i][j].end();
}

const u_int32_t& TMrfData_AFEI3::getActivePixCols() const
{
	_activecolumns = 0;
	setIntBit(0, _activecolumns, getGlobRegValue("EnableCol0"));
	setIntBit(1, _activecolumns, getGlobRegValue("EnableCol0"));
	setIntBit(2, _activecolumns, getGlobRegValue("EnableCol1"));
	setIntBit(3, _activecolumns, getGlobRegValue("EnableCol1"));
	setIntBit(4, _activecolumns, getGlobRegValue("EnableCol2"));
	setIntBit(5, _activecolumns, getGlobRegValue("EnableCol2"));
	setIntBit(6, _activecolumns, getGlobRegValue("EnableCol3"));
	setIntBit(7, _activecolumns, getGlobRegValue("EnableCol3"));
	setIntBit(8, _activecolumns, getGlobRegValue("EnableCol4"));
	setIntBit(9, _activecolumns, getGlobRegValue("EnableCol4"));
	setIntBit(10, _activecolumns, getGlobRegValue("EnableCol5"));
	setIntBit(11, _activecolumns, getGlobRegValue("EnableCol5"));
	setIntBit(12, _activecolumns, getGlobRegValue("EnableCol6"));
	setIntBit(13, _activecolumns, getGlobRegValue("EnableCol6"));
	setIntBit(14, _activecolumns, getGlobRegValue("EnableCol7"));
	setIntBit(15, _activecolumns, getGlobRegValue("EnableCol7"));
	setIntBit(16, _activecolumns, getGlobRegValue("EnableCol8"));
	setIntBit(17, _activecolumns, getGlobRegValue("EnableCol8"));
	return _activecolumns;
}

const u_int32_t& TMrfData_AFEI3::getActivePixColCount() const
{
	_activecolcount = 0;
	_activecolcount += getGlobRegValue("EnableCol0");
	_activecolcount += getGlobRegValue("EnableCol1");
	_activecolcount += getGlobRegValue("EnableCol2");
	_activecolcount += getGlobRegValue("EnableCol3");
	_activecolcount += getGlobRegValue("EnableCol4");
	_activecolcount += getGlobRegValue("EnableCol5");
	_activecolcount += getGlobRegValue("EnableCol6");
	_activecolcount += getGlobRegValue("EnableCol7");
	_activecolcount += getGlobRegValue("EnableCol8");
	_activecolcount *= 2;
	return _activecolcount;
}

bool TMrfData_AFEI3::getPixColActivated(const u_int32_t& column) const
{
	return getIntBit(column, getActivePixCols());
}

const u_int32_t& TMrfData_AFEI3::getNumPixCols()
{
	return mrf_afei3_reglength::logpixcols;
}

const u_int32_t& TMrfData_AFEI3::getNumPixRows()
{
	return mrf_afei3_reglength::logpixrows;
}

void TMrfData_AFEI3::setCommandRegValue(const std::string& item, const mrf::registertype& value)
{
	cmdreg.find(item)->second.value = value;
}

void TMrfData_AFEI3::setDefaultAddress(const mrf::registertype& address)
{
	feaddress = address;
	setCommandRegValue("Address", address);
}

void TMrfData_AFEI3::setDefaultBroadcast(const mrf::registertype& broadcast)
{
	febroadcast = broadcast;
	setCommandRegValue("Broadcast", broadcast);
}

const mrf::registertype& TMrfData_AFEI3::getDefaultAddress() const
{
	return feaddress;
}

const mrf::registertype& TMrfData_AFEI3::getDefaultBroadcast() const
{
	return febroadcast;
}

void TMrfData_AFEI3::setGlobRegValue(const std::string& item, const mrf::registertype& value)
{
	globreg.find(item)->second.value = value;
}

void TMrfData_AFEI3::setPixRegValue(const std::string& item, const u_int32_t& i, const u_int32_t& j, const mrf::registertype& value)
{
	if (item == mrf_afei3::fdac) {
		pixreg[i][j].find(item + "0")->second.value = static_cast<u_int32_t>(getIntBit(0, value));
		pixreg[i][j].find(item + "1")->second.value = static_cast<u_int32_t>(getIntBit(1, value));
		pixreg[i][j].find(item + "2")->second.value = static_cast<u_int32_t>(getIntBit(2, value));
	} else if (item == mrf_afei3::tdac) {
		pixreg[i][j].find(item + "0")->second.value = static_cast<u_int32_t>(getIntBit(0, value));
		pixreg[i][j].find(item + "1")->second.value = static_cast<u_int32_t>(getIntBit(1, value));
		pixreg[i][j].find(item + "2")->second.value = static_cast<u_int32_t>(getIntBit(2, value));
		pixreg[i][j].find(item + "3")->second.value = static_cast<u_int32_t>(getIntBit(3, value));
		pixreg[i][j].find(item + "4")->second.value = static_cast<u_int32_t>(getIntBit(4, value));
		pixreg[i][j].find(item + "5")->second.value = static_cast<u_int32_t>(getIntBit(5, value));
		pixreg[i][j].find(item + "6")->second.value = static_cast<u_int32_t>(getIntBit(6, value));
	} else {
		pixreg[i][j].find(item)->second.value = static_cast<u_int32_t>(value > 0);
	}
}

const mrf::registertype& TMrfData_AFEI3::getCommandRegValue(const std::string& item) const
{
	return cmdreg.find(item)->second.value;
}

const mrf::registertype& TMrfData_AFEI3::getGlobRegValue(const std::string& item) const
{
	return globreg.find(item)->second.value;
}

const mrf::registertype& TMrfData_AFEI3::getPixRegValue(const std::string& item, const u_int32_t& i, const u_int32_t& j) const
{
	if (item == mrf_afei3::fdac) {
		_pixregvalue = 0;
		setIntBit(0, _pixregvalue, pixreg[i][j].find(item + "0")->second.value > 0);
		setIntBit(1, _pixregvalue, pixreg[i][j].find(item + "1")->second.value > 0);
		setIntBit(2, _pixregvalue, pixreg[i][j].find(item + "2")->second.value > 0);
		return _pixregvalue;
	} else if (item == mrf_afei3::tdac) {
		_pixregvalue = 0;
		setIntBit(0, _pixregvalue, pixreg[i][j].find(item + "0")->second.value > 0);
		setIntBit(1, _pixregvalue, pixreg[i][j].find(item + "1")->second.value > 0);
		setIntBit(2, _pixregvalue, pixreg[i][j].find(item + "2")->second.value > 0);
		setIntBit(3, _pixregvalue, pixreg[i][j].find(item + "3")->second.value > 0);
		setIntBit(4, _pixregvalue, pixreg[i][j].find(item + "4")->second.value > 0);
		setIntBit(5, _pixregvalue, pixreg[i][j].find(item + "5")->second.value > 0);
		setIntBit(6, _pixregvalue, pixreg[i][j].find(item + "6")->second.value > 0);
		return _pixregvalue;
	} else {
		return pixreg[i][j].find(item)->second.value;
	}
}

void TMrfData_AFEI3::updatePixRegPosCache() const
{
	if (pixRegPosCacheExpired()) {
		for (unsigned i = 0; i < getNumPixCols(); ++i) {
			if (getPixColActivated(i)) {
				for (unsigned j = 0; j < getNumPixRows(); ++j) {
					_pixregpos[i][j] = getPixRegPositionFromCoords(i, j);
					_pixregcol[_pixregpos[i][j]] = i;
					_pixregrow[_pixregpos[i][j]] = j;
				}
			}
		}
		_activecolcache = getActivePixCols();
	}
}

bool TMrfData_AFEI3::pixRegPosCacheExpired() const
{
	return (_activecolcache != getActivePixCols());
}

u_int32_t TMrfData_AFEI3::getPixRegPositionFromCoords(const u_int32_t& col, const u_int32_t& row) const
{
	u_int32_t relcolpos = 0;
	for (u_int32_t i = 0; i < col; ++i) {
		if (getPixColActivated(i)) {
			++relcolpos;
		}
	}
	if ((relcolpos % 2) == 0) {
		return (getActivePixColCount() - relcolpos)*getNumPixRows() - row - 1;
	} else {
		return (getActivePixColCount() - relcolpos - 1) * getNumPixRows() + row;
	}
}

const u_int32_t& TMrfData_AFEI3::getPixRegPositionFromCoordsFast(const u_int32_t& col, const u_int32_t& row) const
{
	return _pixregpos[col][row];
}

u_int32_t TMrfData_AFEI3::getPixColFromPixRegPosition(const u_int32_t& position) const
{
	int relcolpos = 1 + (position / getNumPixRows());
	u_int32_t abscolpos = getNumPixCols();
	while ((abscolpos > 0) && (relcolpos > 0)) {
		--abscolpos;
		if (getPixColActivated(abscolpos)) {
			--relcolpos;
		}
	}
	if (relcolpos == 0) {
		return abscolpos;
	} else {
		errcode |= mrfdata_error::out_of_bounds;
		return 0;
	}
}

const u_int32_t& TMrfData_AFEI3::getPixColFromPixRegPositionFast(const u_int32_t& position) const
{
	return _pixregcol[position];
}

u_int32_t TMrfData_AFEI3::getPixRowFromPixRegPosition(const u_int32_t& position) const
{
	if (((position) / getNumPixRows()) % 2 == 0) {
		return static_cast<u_int32_t>(position % getNumPixRows());
	} else {
		return static_cast<u_int32_t>(getNumPixRows() - (position % getNumPixRows()) - 1);
	}
}

const u_int32_t& TMrfData_AFEI3::getPixRowFromPixRegPositionFast(const u_int32_t& position) const
{
	return _pixregrow[position];
}

void TMrfData_AFEI3::initMaps()
{
	initCommandReg();
	initGlobReg();
	initPixReg();
}





