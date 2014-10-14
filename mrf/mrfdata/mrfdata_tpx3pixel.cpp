/*============================================================*/
/* mrfdata_tpxpixel.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpx3pixel.h"
#include "mrf_confitem.h"
#include "mrfdataadv1d.h"

TMrfData_Tpx3Pixel::TMrfData_Tpx3Pixel(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
	initMaps();
}

void TMrfData_Tpx3Pixel::initMaps()
{
	_localdata.clear();
        _localdata["PixelMask"]                 = TConfItem(0, 0, 1);
        _localdata["TestPulsEnable"]            = TConfItem(0, 1, 1);
        _localdata["ComparatorTestOutEnable"]   = TConfItem(0, 2, 1);
        _localdata["ThresholdDAC"]              = TConfItem(0, 3, 4);
        _localdata["ThresholdDACSign"]          = TConfItem(0, 7, 1);
        _localdata["NotUsed"]                   = TConfItem(0, 8, 4);
        _localdata["OperationCode"]             = TConfItem(0,12, 5);
        _localdata["Padding"]                   = TConfItem(0,17,15);

	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
}

void TMrfData_Tpx3Pixel::assemble()
{
    if (getNumBits() < _datastreamlength) {
            setNumBits(_datastreamlength);
    }
        setStreamConfItemValue(_localdata.find("PixelMask")->second);
        setStreamConfItemValue(_localdata.find("TestPulsEnable")->second);
        setStreamConfItemValue(_localdata.find("ComparatorTestOutEnable")->second);
        setStreamConfItemValue(_localdata.find("ThresholdDAC")->second);
        setStreamConfItemValue(_localdata.find("ThresholdDACSign")->second);
        setStreamConfItemValue(_localdata.find("NotUsed")->second);
        setStreamConfItemValue(_localdata.find("OperationCode")->second);
        setStreamConfItemValue(_localdata.find("Padding")->second);
}

void TMrfData_Tpx3Pixel::setCommand(u_int32_t command)
{
        _localdata.find("OperationCode")->second.value = command;
}
