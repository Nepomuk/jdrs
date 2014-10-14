/*============================================================*/
/* mrfdata_tpxpixel.cpp                                       */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfdata_tpxpixel.h"
#include <string>
#include <vector>
#include <map>
#include "mrf_confitem.h"
#include "mrfdataadv1d.h"
#include <iostream>

TMrfData_TpxPixel::TMrfData_TpxPixel(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
	_colcount(topix::colcount)
{
	initMaps();
}

void TMrfData_TpxPixel::initMaps()
{
	_rowcount.push_back(topix::longrowcount);
	_rowcount.push_back(topix::longrowcount);
	_rowcount.push_back(topix::shortrowcount);
	_rowcount.push_back(topix::shortrowcount);

	_localdata.clear();
	_localdata["Mask"] = TConfItem(0, 0, 1);
	_localdata["TDAC"] = TConfItem(0, 1, 4);
	_localdata["DACPolarity"] = TConfItem(0, 5, 1);
	_localdata["CompToVout"] = TConfItem(0, 6, 1);
	_localdata["CalDisable"] = TConfItem(0, 7, 1);
	_localdata["SensorType"] = TConfItem(0, 8, 4);
	_datastreamlength = 0;
	std::map<std::string, TConfItem>::const_iterator iter;
	for (iter = getItemIteratorBegin(); iter != getItemIteratorEnd(); ++iter) {
		_datastreamlength += iter->second.length;
	}
	_pixregs.clear();
	_pixregs.resize(_colcount);
	for (unsigned int i = 0; i < _colcount; ++i) {
		_pixregs.at(i).resize(getNumRows(i), _localdata);
	}
}

void TMrfData_TpxPixel::setPixConf(const unsigned int& col, const unsigned int& row, const std::string& item, const u_int32_t& value)
{
	_pixregs.at(col).at(row).find(item)->second.value = value;
}

const u_int32_t& TMrfData_TpxPixel::getPixConf(const unsigned int& col, const unsigned int& row, const std::string& item) const
{
	return _pixregs.at(col).at(row).find(item)->second.value;
}

void TMrfData_TpxPixel::assembleCol(const unsigned int& col)
{
	clearDataStream();
	setNumWords(getNumRows(col));
	TMrfDataAdv1D::constItemIterator iter;
	for (unsigned int i = 0; i < getNumRows(col); ++i) {
		for (iter = _pixregs.at(col).at(i).begin(); iter != _pixregs.at(col).at(i).end(); ++iter) {
			setStreamConfItemValue(iter->second, iter->second.value, i);
		}
	}
}

const unsigned int& TMrfData_TpxPixel::getNumCols() const
{
	return _colcount;
}

const unsigned int& TMrfData_TpxPixel::getNumRows(const unsigned int& col) const
{
	return _rowcount.at(col);
}

TMrfDataAdv1D::itemMap& TMrfData_TpxPixel::getPixel(const unsigned int& col, const unsigned int& row)
{
	return _pixregs.at(col).at(row);
}
