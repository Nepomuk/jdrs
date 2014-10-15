/*============================================================*/
/* mrftal_agilent33250a.cpp                                   */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Agilent 33250a Pulsgenerator    */
/*                                                  S.U. Esch */
/*============================================================*/
#include "mrftal_agilent33250a.h"

TMrfTal_Agilent33250a::TMrfTal_Agilent33250a()
:TMrfGal(), TMrfGal_CharacterDevice(), TMrfGal_SerialPort()
{
}

void TMrfTal_Agilent33250a::doTrigger()
{
	write("TRIG\n");
}

void TMrfTal_Agilent33250a::doBeep()
{
	write("SYST:BEEP\n");
}

void TMrfTal_Agilent33250a::openDevice(const char* const devicefile)
{
	TMrfGal_CharacterDevice::openDevice(devicefile);
	setDeviceConfiguration(mrf_serialport_config::BAUD_9600,  mrf_serialport_config::PARITY_NONE ,mrf_serialport_config::CHAR_SIZE_8);
}
