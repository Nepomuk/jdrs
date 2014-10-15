/*============================================================*/
/* mrfdata_chainltc.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides access to LTC DAC chain configuration.            */
/*                                                   S. Esch  */
/*============================================================*/

#ifndef __MRFDATA_CHAINLTC_H__
#define __MRFDATA_CHAINLTC_H__

#include "mrfdataadv2d.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

namespace chainltc2620 {
	static const u_int32_t write = 0;
	static const u_int32_t update = 1;
	static const u_int32_t writeupdateall = 2;
	static const u_int32_t writeupdate = 3;
	static const u_int32_t powerdown = 4;
	static const u_int32_t noop = 15;
	static const u_int32_t daca = 0;
	static const u_int32_t dacb = 1;
	static const u_int32_t dacc = 2;
	static const u_int32_t dacd = 3;
	static const u_int32_t dace = 4;
	static const u_int32_t dacf = 5;
	static const u_int32_t dacg = 6;
	static const u_int32_t dach = 7;
	static const u_int32_t alldacs = 15;
}
namespace chainltc2604 {
        static const u_int32_t write = 0;
        static const u_int32_t update = 1;
        static const u_int32_t writeupdateall = 2;
        static const u_int32_t writeupdate = 3;
        static const u_int32_t powerdown = 4;
        static const u_int32_t noop = 15;
        static const u_int32_t daca = 0;
        static const u_int32_t dacb = 1;
        static const u_int32_t dacc = 2;
        static const u_int32_t dacd = 3;
        static const u_int32_t alldacs = 15;
}


class TMrfData_ChainLTC : virtual public TMrfDataAdv2D
{
	public:
        TMrfData_ChainLTC(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);

            virtual void setCommands(const u_int32_t& command1, const u_int32_t& command2);
            virtual void setCommand(const std::string& ltc, const u_int32_t& command);
            virtual void setDACValues(const std::string& dac1, const u_int32_t& value1,const std::string& dac2, const u_int32_t& value2);
            virtual void setDACValue(const std::string& ltc,const std::string& dac1, const u_int32_t& value1);
            virtual u_int32_t getDACValue(const std::string& ltc, const std::string& dac);
            virtual void setDACActivated(const std::string& ltc, const std::string& dac, const u_int32_t& active);
            virtual u_int32_t getDACActivated(const std::string& ltc, const std::string& dac);
		
            virtual void assembleDAC(const std::string& dac);

            virtual std::map<std::string, std::map< std::string, TConfItem> >::iterator getLTCIteratorBegin();
            virtual std::map<std::string, std::map< std::string, TConfItem> >::iterator getLTCIteratorEnd();

            virtual std::map<std::string, std::map< std::string, TConfItem> >::iterator getLTCActivatedIteratorBegin();
            virtual std::map<std::string, std::map< std::string, TConfItem> >::iterator getLTCActivatedIteratorEnd();

            virtual std::map<std::string, TConfItem>::iterator getDACIteratorBegin(const std::string& ltc);
            virtual std::map<std::string, TConfItem>::iterator getDACIteratorEnd(const std::string& ltc);
            virtual std::map<std::string, TConfItem>::iterator getDACActivatedIteratorBegin(const std::string& ltc);
            virtual std::map<std::string, TConfItem>::iterator getDACActivatedIteratorEnd(const std::string& ltc);

            virtual std::map<std::string, TConfItem>::const_iterator getConstDACIteratorBegin(const std::string& ltc) const;
            virtual std::map<std::string, TConfItem>::const_iterator getConstDACIteratorEnd(const std::string& ltc) const;
            virtual std::map<std::string, TConfItem>::const_iterator getConstDACActivatedIteratorBegin(const std::string& ltc) const;
            virtual std::map<std::string, TConfItem>::const_iterator getConstDACActivatedIteratorEnd(const std::string& ltc) const;
            virtual int getDACCount(const std::string& ltc) const;

            //From TMrfDataAdvBase
            virtual void initMaps()=0;

	protected:
            //std::map<std::string, TConfItem > _ltcdacvalues;
            //std::map<std::string, TConfItem > _ltcdacactivated;
            //std::map<std::string, TConfItem > _ltcdacaddress;

            std::map<std::string, std::map<std::string, TConfItem> > _ltcdacvalues;
            std::map<std::string, std::map<std::string, TConfItem> > _ltcdacactivated;
            std::map<std::string, std::map<std::string, TConfItem> > _ltcdacaddress;

        private:


};


#endif // __MRFDATA_CHAINLTC_H__
