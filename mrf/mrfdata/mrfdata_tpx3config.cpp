/*============================================================*/
/* mrfdata_tpxpixel.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpx3config.h"
#include "mrf_confitem.h"
#include "mrfdataadv1d.h"

TMrfData_Tpx3Config::TMrfData_Tpx3Config(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_Tpx3Config::initMaps()
{
	_localdata.clear();

        // CCR0

        _localdata["LeadingEdgeOnly"]       = TConfItem(0, 0, 1);
        _localdata["CounterMode"]           = TConfItem(0, 1, 1);
        _localdata["CounterEnable"]         = TConfItem(0, 2, 1);
        _localdata["ReadoutCycleHalfSpeed"] = TConfItem(0, 3, 1);
        _localdata["DriverPreemphasis"]     = TConfItem(0, 4, 1);
        _localdata["SLVSCurrentControl"]    = TConfItem(0, 5, 4);
        _localdata["FreezeStop"]            = TConfItem(0, 9, 3);
        _localdata["CommandCCR0"]           = TConfItem(0,12, 5);
        _localdata["PaddingCCR0"]           = TConfItem(0,17,15);

        // CCR1

        _localdata["Leak_N"]                = TConfItem(0,32, 1);
        _localdata["Leak_P"]                = TConfItem(0,33, 1);
        _localdata["SelectPol"]             = TConfItem(0,34, 1);
        _localdata["IleakCtrl1"]            = TConfItem(0,35, 1);
        _localdata["IleakCtrl2"]            = TConfItem(0,36, 1);
        _localdata["AnalogTimeoutEnable"]   = TConfItem(0,37, 1);
        _localdata["NotUsed"]               = TConfItem(0,38, 6);
        _localdata["CommandCCR1"]           = TConfItem(0,44, 5);
        _localdata["PaddingCCR1"]           = TConfItem(0,49,15);

        //CCR2

        _localdata["CounterStopValue"]      = TConfItem(0,64,12);
        _localdata["CommandCCR2"]           = TConfItem(0,76, 5);
        _localdata["PaddingCCR2"]           = TConfItem(0,81,15);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}

void TMrfData_Tpx3Config::assemble()
{
    if (getNumBits() < _datastreamlength) {
            setNumBits(_datastreamlength);
    }

    setStreamConfItemValue(_localdata.find("LeadingEdgeOnly")->second);
    setStreamConfItemValue(_localdata.find("CounterMode")->second);
    setStreamConfItemValue(_localdata.find("CounterEnable")->second);
    setStreamConfItemValue(_localdata.find("ReadoutCycleHalfSpeed")->second);
    setStreamConfItemValue(_localdata.find("DriverPreemphasis")->second);
    setStreamConfItemValue(_localdata.find("SLVSCurrentControl")->second);
    setStreamConfItemValue(_localdata.find("FreezeStop")->second);
    setStreamConfItemValue(_localdata.find("CommandCCR0")->second);
    setStreamConfItemValue(_localdata.find("PaddingCCR0")->second);

    setStreamConfItemValue(_localdata.find("Leak_N")->second);
    setStreamConfItemValue(_localdata.find("Leak_P")->second);
    setStreamConfItemValue(_localdata.find("SelectPol")->second);
    setStreamConfItemValue(_localdata.find("IleakCtrl1")->second);
    setStreamConfItemValue(_localdata.find("IleakCtrl2")->second);
    setStreamConfItemValue(_localdata.find("AnalogTimeoutEnable")->second);
    setStreamConfItemValue(_localdata.find("NotUsed")->second);
    setStreamConfItemValue(_localdata.find("CommandCCR1")->second);
    setStreamConfItemValue(_localdata.find("PaddingCCR1")->second);

    setStreamConfItemValue(_localdata.find("CounterStopValue")->second);
    setStreamConfItemValue(_localdata.find("CommandCCR2")->second);
    setStreamConfItemValue(_localdata.find("PaddingCCR2")->second);
}

//void TMrfData_Tpx3Config::disassemble()
//{


//}

void TMrfData_Tpx3Config::setCommand(std::string ccr, u_int32_t  command)
{
    _localdata.find(ccr)->second.value = command;
}
