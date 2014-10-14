/*============================================================*/
/* mrfcal_topix.cpp                                           */
/* MVD Readout Framework Chip Access Layer                    */
/* Chip Access Layer for Topix 3 Frontend Prototype           */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfcal_topix3.h"
#include "mrftal_rbtpx3.h"
#include "mrfcal.h"
#include "mrfdata_chain2ltc2604.h"
#include "mrfdata_tpxmasterreg.h"
#include <map>
#include <iostream>

TMrfCal_Topix3::TMrfCal_Topix3()
: TMrfCal(), TMrfTal_RBTopix3()
{
}

TMrfCal_Topix3::~TMrfCal_Topix3()
{
}

void TMrfCal_Topix3::configLTC(TMrfData_Chain2LTC2604 data) const
{
       std::map<std::string, TConfItem>::const_iterator iterDAC;
       std::map<const std::string, std::map<std::string, TConfItem> >::const_iterator iterLTC;
       for(iterDAC = data.getDACIteratorBegin("LTC1"); iterDAC !=data.getDACIteratorEnd("LTC1"); ++iterDAC){
           for(iterLTC = data.getLTCIteratorBegin(); iterLTC!= data.getLTCIteratorEnd(); ++iterLTC){
               if(data.getDACActivated(iterLTC->first, iterDAC->first)==1)
               {
                   data.setCommand(iterLTC->first,ltc2604::writeupdate);
               }
               else
               {
                   data.setCommand(iterLTC->first ,ltc2604::powerdown);
               }
           }
           data.assembleDAC(iterDAC->first);
           writeLTCData(data);
       }
}


void TMrfCal_Topix3::configCCR(TMrfData_Tpx3Config data) const
{
        writeRemoteData(data);    
//        std::cerr <<"data.getNumBits() "<< data.getNumBits() << std::endl;
//        std::cerr <<"data.exportBinString() "<< data.exportBinString() << std::endl;
//        boardCommand(tpxctrl_value::topix3config);
        startSerializer(18);
        read(tpx_address::sdataout);
        read(tpx_address::sdataout);
        read(tpx_address::sdataout);
}

void TMrfCal_Topix3::writeToChip(const TMrfData& data) const
{
        writeRemoteData(data);
        boardCommand(tpxctrl_value::topix3config);

       // startSerializer(11);

//    boardCommand()

//	writeRemoteData(data);
//	startShifter(data.getNumWords());
}

void TMrfCal_Topix3::readFromChip(TMrfData& data)
{
	triggerRead(data.getNumWords(), true);
	readRemoteData(data);
}

void TMrfCal_Topix3::readCCR0(TMrfData_Tpx3Config& data) const
{
        data.setCommand(topix3_ccrnumber::ccr0,topix3_command::readdataccr0);
        data.assemble();
        write(tpx_address::input,data.getWord(0));
        data.setCommand(topix3_ccrnumber::ccr0,topix3_command::nooperation);
        data.assemble();
        write(tpx_address::input,data.getWord(0));
        boardCommand(tpxctrl_value::topix3config);
        data.setWord(0,read(tpx_address::sdataout));
        data.setWord(0,read(tpx_address::sdataout));
}

void TMrfCal_Topix3::readCCR1(TMrfData_Tpx3Config& data) const
{
        data.setCommand(topix3_ccrnumber::ccr1,topix3_command::readdataccr1);
        data.assemble();
        write(tpx_address::input,data.getWord(1));
        data.setCommand(topix3_ccrnumber::ccr1, topix3_command::nooperation);
        data.assemble();
        write(tpx_address::input, data.getWord(1));
        boardCommand(tpxctrl_value::topix3config);
        data.setWord(1,read(tpx_address::sdataout));
        data.setWord(1,read(tpx_address::sdataout));
}
void TMrfCal_Topix3::readCCR2(TMrfData_Tpx3Config& data) const
{
        data.setCommand(topix3_ccrnumber::ccr2,topix3_command::readdataccr2);
        data.assemble();
        write(tpx_address::input,data.getWord(2));
        data.setCommand(topix3_ccrnumber::ccr2,topix3_command::nooperation);
        data.assemble();
        write(tpx_address::input,data.getWord(2));
        boardCommand(tpxctrl_value::topix3config);
        data.setWord(2,read(tpx_address::sdataout));
        data.setWord(2,read(tpx_address::sdataout));
}

void TMrfCal_Topix3::writeCommand(TMrfData_Tpx3Config &data) const
{
        data.assemble();
        write(tpx_address::input,data.getWord(0));
        boardCommand(tpxctrl_value::topix3config);
}


