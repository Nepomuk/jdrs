/*============================================================*/
/* mrfdata_afei3hits.cpp                                      */
/* MVD Readout Framework Hit Data Storage                     */
/* Specific to Atlas FE-I3 Registers                          */
/*                                               M.C. Mertens */
/*============================================================*/

#include <map>
#include <string>
#include "mrfdata_afei3hits.h"
#include "mrf_confitem.h"

TMrfData_AFEI3Hits::TMrfData_AFEI3Hits()
{
	initMaps();
}

const std::string TMrfData_AFEI3Hits::_eoep = "EOEWordP";
const std::string TMrfData_AFEI3Hits::_hitp = "HitWordP";
const std::string TMrfData_AFEI3Hits::_eoe = "EOEWord";
const std::string TMrfData_AFEI3Hits::_hit = "HitWord";

void TMrfData_AFEI3Hits::initMaps()
{
	//maplist.clear();
	initEOEStructure();
	initEOEPStructure();
	initHitStructure();
	initHitPStructure();
	/*
	maplist["EOEWord"] = eoeword;
	maplist["EOEWordP"] = eoewordp;
	maplist["HitWord"] = hitword;
	maplist["HitWordP"] = hitwordp;
	*/
}

void TMrfData_AFEI3Hits::initEOEStructure()
{
	maplist["EOEWord"].clear();
	maplist["EOEWord"]["Header"] = TConfItem(0, 0, 1);
	maplist["EOEWord"]["BCID1"] = TConfItem(0, 1, 4);
	maplist["EOEWord"]["Flag"] = TConfItem(0, 5, 8);
	maplist["EOEWord"]["Zero"] = TConfItem(0, 13, 1);
	maplist["EOEWord"]["L1ID"] = TConfItem(0, 14, 4);
	maplist["EOEWord"]["BCID2"] = TConfItem(0, 18, 4);
	maplist["EOEWord"]["Warn"] = TConfItem(0, 22, 4);
	maplist["EOEWord"]["IsEOE"] = TConfItem(0, 5, 3);
	/*
	eoeword.clear();
	eoeword["Header"] = TConfItem(0, 0, 1);
	eoeword["BCID1"] = TConfItem(0, 1, 4);
	eoeword["Flag"] = TConfItem(0, 5, 8);
	eoeword["Zero"] = TConfItem(0, 13, 1);
	eoeword["L1ID"] = TConfItem(0, 14, 4);
	eoeword["BCID2"] = TConfItem(0, 18, 4);
	eoeword["Warn"] = TConfItem(0, 22, 4);
	*/
}

void TMrfData_AFEI3Hits::initEOEPStructure()
{
	maplist["EOEWordP"].clear();
	maplist["EOEWordP"]["Header"] = TConfItem(0, 0, 1);
	maplist["EOEWordP"]["BCID1"] = TConfItem(0, 1, 4);
	maplist["EOEWordP"]["Flag"] = TConfItem(0, 5, 8);
	maplist["EOEWordP"]["Parity"] = TConfItem(0, 13, 1);
	maplist["EOEWordP"]["L1ID"] = TConfItem(0, 14, 4);
	maplist["EOEWordP"]["BCID2"] = TConfItem(0, 18, 4);
	maplist["EOEWordP"]["Warn"] = TConfItem(0, 22, 4);
	maplist["EOEWordP"]["IsEOE"] = TConfItem(0, 5, 3);
	/*
	eoewordp.clear();
	eoewordp["Header"] = TConfItem(0, 0, 1);
	eoewordp["BCID1"] = TConfItem(0, 1, 4);
	eoewordp["Flag"] = TConfItem(0, 5, 8);
	eoewordp["Parity"] = TConfItem(0, 13, 1);
	eoewordp["L1ID"] = TConfItem(0, 14, 4);
	eoewordp["BCID2"] = TConfItem(0, 18, 4);
	eoewordp["Warn"] = TConfItem(0, 22, 4);
	*/
}

void TMrfData_AFEI3Hits::initHitStructure()
{
	maplist["HitWord"].clear();
	maplist["HitWord"]["Header"] = TConfItem(0, 0, 1);
	maplist["HitWord"]["BCID"] = TConfItem(0, 1, 4);
	maplist["HitWord"]["Row"] = TConfItem(0, 5, 8);
	maplist["HitWord"]["Col"] = TConfItem(0, 13, 5);
	maplist["HitWord"]["TOT"] = TConfItem(0, 18, 8);
	maplist["HitWord"]["IsEOE"] = TConfItem(0, 5, 3);
	/*
	hitword.clear();
	hitword["Header"] = TConfItem(0, 0, 1);
	hitword["BCID"] = TConfItem(0, 1, 4);
	hitword["Row"] = TConfItem(0, 5, 8);
	hitword["Col"] = TConfItem(0, 13, 5);
	hitword["TOT"] = TConfItem(0, 18, 8);
	*/
}

void TMrfData_AFEI3Hits::initHitPStructure()
{
	maplist["HitWordP"].clear();
	maplist["HitWordP"]["Header"] = TConfItem(0, 0, 1);
	maplist["HitWordP"]["BCID"] = TConfItem(0, 1, 4);
	maplist["HitWordP"]["Row"] = TConfItem(0, 5, 8);
	maplist["HitWordP"]["Col"] = TConfItem(0, 13, 5);
	maplist["HitWordP"]["Parity"] = TConfItem(0, 18, 1);
	maplist["HitWordP"]["TOT"] = TConfItem(0, 19, 7);
	maplist["HitWordP"]["IsEOE"] = TConfItem(0, 5, 3);
	/*
	hitwordp.clear();
	hitwordp["Header"] = TConfItem(0, 0, 1);
	hitwordp["BCID"] = TConfItem(0, 1, 4);
	hitwordp["Row"] = TConfItem(0, 5, 8);
	hitwordp["Col"] = TConfItem(0, 13, 5);
	hitwordp["Parity"] = TConfItem(0, 18, 1);
	hitwordp["TOT"] = TConfItem(0, 19, 7);
	*/
}

const u_int32_t& TMrfData_AFEI3Hits::getItemValue(const std::string& source, const std::string& item, const u_int32_t& index) const
{
	return getBitBlock((index *  bitsinablock) + 6 + maplist.find(source)->second.find(item)->second.position, maplist.find(source)->second.find(item)->second.length, 0, true);
	//return getBitBlock((i *  bitsinablock) + 7 + hitword.find(item)->second.position, hitword.find(item)->second.length);
	//return hitword.find(item)->second.position;
}

const std::string& TMrfData_AFEI3Hits::getDataType(const u_int32_t& index, const u_int32_t& parity) const
{
	if (parity == 0) {
		if (getItemValue(_hit, "IsEOE", index) == 7) {
			return _eoe;
		} else {
			return _hit;
		}
	} else {
		if (getItemValue(_hit, "IsEOE", index) == 7) {
			return _eoep;
		} else {
			return _hitp;
		}
	}
}

bool TMrfData_AFEI3Hits::isEOEWord(const u_int32_t& index) const
{
	return (getItemValue(_hit, "IsEOE", index) == 7);
}

bool TMrfData_AFEI3Hits::isHitWord(const u_int32_t& index) const
{
	return (getItemValue(_hit, "IsEOE", index) != 7);
}

const std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator TMrfData_AFEI3Hits::getTypeIteratorBegin() const
{
	return maplist.begin();
}

const std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator TMrfData_AFEI3Hits::getTypeIteratorEnd() const
{
	return maplist.end();
}


const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3Hits::getItemIteratorBegin(const std::string& type) const
{
	return maplist.find(type)->second.begin();
}

const std::map<const std::string, TConfItem>::const_iterator TMrfData_AFEI3Hits::getItemIteratorEnd(const std::string& type) const
{
	return maplist.find(type)->second.end();
}


