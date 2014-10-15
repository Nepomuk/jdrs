/*============================================================*/
/* mrfdata_chain2ltc2620.h                                    */
/* MVD Readout Framework Data Storage                         */
/* Provides access to two LTC2604 DAC daisychain              */
/* configuration.                                             */
/*                                                    S. Esch */
/*============================================================*/

#include "mrfdata_chain2ltc2604.h"
#include <map>
#include <string>
#include "mrf_confitem.h"
#include <iostream>

TMrfData_Chain2LTC2604::TMrfData_Chain2LTC2604(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdvBase(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
  TMrfDataAdv2D(blocklength)
{
	initMaps();
}

void TMrfData_Chain2LTC2604::initMaps()
{
        _localdata.clear();
        _localdata["LTC1"]["Padding"] = TConfItem(0, 0, 8);
        _localdata["LTC1"]["Command"] = TConfItem(0, 8, 4);
        _localdata["LTC1"]["Address"] = TConfItem(0, 12, 4);
        _localdata["LTC1"]["Data"]    = TConfItem(0, 16, 16);
        _localdata["LTC2"]["Padding"] = TConfItem(0, 32, 8);
        _localdata["LTC2"]["Command"] = TConfItem(0, 40, 4);
        _localdata["LTC2"]["Address"] = TConfItem(0, 44, 4);
        _localdata["LTC2"]["Data"]    = TConfItem(0, 48, 16);


        _datastreamlength = 0;
        std::map<std::string, std::map<std::string, TConfItem> >::const_iterator iter;
        for (iter = getTypeIteratorBegin(); iter != getTypeIteratorEnd(); ++iter) {
            std::map<std::string, TConfItem>::const_iterator iter2;
            for (iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
                _datastreamlength += iter2->second.length;
                std::cout << "_datastreamlength" << _datastreamlength << std::endl;
            }
        }

	_ltcdacvalues.clear();

        (_ltcdacvalues["LTC1"])["DACA"] = TConfItem(0, 16, 16);
        (_ltcdacvalues["LTC1"])["DACB"] = TConfItem(0, 16, 16);
        (_ltcdacvalues["LTC1"])["DACC"] = TConfItem(0, 16, 16);
        (_ltcdacvalues["LTC1"])["DACD"] = TConfItem(0, 16, 16);
        (_ltcdacvalues["LTC2"])["DACA"] = TConfItem(0, 48, 16);
        (_ltcdacvalues["LTC2"])["DACB"] = TConfItem(0, 48, 16);
        (_ltcdacvalues["LTC2"])["DACC"] = TConfItem(0, 48, 16);
        (_ltcdacvalues["LTC2"])["DACD"] = TConfItem(0, 48, 16);

	_ltcdacactivated.clear();

        (_ltcdacactivated["LTC1"])["DACA"] = TConfItem(0, 0, 1);
        (_ltcdacactivated["LTC1"])["DACB"] = TConfItem(0, 0, 1);
        (_ltcdacactivated["LTC1"])["DACC"] = TConfItem(0, 0, 1);
        (_ltcdacactivated["LTC1"])["DACD"] = TConfItem(0, 0, 1);
        (_ltcdacactivated["LTC2"])["DACA"] = TConfItem(0, 32, 1);
        (_ltcdacactivated["LTC2"])["DACB"] = TConfItem(0, 32, 1);
        (_ltcdacactivated["LTC2"])["DACC"] = TConfItem(0, 32, 1);
        (_ltcdacactivated["LTC2"])["DACD"] = TConfItem(0, 32, 1);

	_ltcdacaddress.clear();

        (_ltcdacaddress["LTC1"])["DACA"] = TConfItem(chainltc2604::daca, 12, 4);
        (_ltcdacaddress["LTC1"])["DACB"] = TConfItem(chainltc2604::dacb, 12, 4);
        (_ltcdacaddress["LTC1"])["DACC"] = TConfItem(chainltc2604::dacc, 12, 4);
        (_ltcdacaddress["LTC1"])["DACD"] = TConfItem(chainltc2604::dacd, 12, 4);
        (_ltcdacaddress["LTC2"])["DACA"] = TConfItem(chainltc2604::daca, 44, 4);
        (_ltcdacaddress["LTC2"])["DACB"] = TConfItem(chainltc2604::dacb, 44, 4);
        (_ltcdacaddress["LTC2"])["DACC"] = TConfItem(chainltc2604::dacc, 44, 4);
        (_ltcdacaddress["LTC2"])["DACD"] = TConfItem(chainltc2604::dacd, 44, 4);

}
