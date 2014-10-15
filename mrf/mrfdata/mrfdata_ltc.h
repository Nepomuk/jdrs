/*============================================================*/
/* mrfdata_ltc2620.h                                          */
/* MVD Readout Framework Data Storage                         */
/* Provides access to LTC2620 DAC configuration.              */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_LTC_H__
#define __MRFDATA_LTC_H__

#include "mrfdataadv1d.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

namespace ltc2620 {
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
namespace ltc2604 {
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


class TMrfData_LTC : virtual public TMrfDataAdv1D
{
	public:
        TMrfData_LTC(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);

		virtual void setCommand(const u_int32_t& command);
		virtual void setDACValue(const std::string& dac, const u_int32_t& value);
		virtual u_int32_t getDACValue(const std::string& dac);
		virtual void setDACActivated(const std::string& dac, const u_int32_t& active);
		virtual u_int32_t getDACActivated(const std::string& dac);
		
		virtual void assembleDAC(const std::string& dac);

		virtual std::map<std::string, TConfItem>::iterator getDACIteratorBegin();
		virtual std::map<std::string, TConfItem>::iterator getDACIteratorEnd();
		virtual std::map<std::string, TConfItem>::iterator getDACActivatedIteratorBegin();
		virtual std::map<std::string, TConfItem>::iterator getDACActivatedIteratorEnd();

		virtual std::map<std::string, TConfItem>::const_iterator getConstDACIteratorBegin() const;
		virtual std::map<std::string, TConfItem>::const_iterator getConstDACIteratorEnd() const;
		virtual std::map<std::string, TConfItem>::const_iterator getConstDACActivatedIteratorBegin() const;
		virtual std::map<std::string, TConfItem>::const_iterator getConstDACActivatedIteratorEnd() const;
		virtual int getDACCount() const;

		//From TMrfDataAdvBase
        virtual void initMaps()=0;

	protected:
        std::map<std::string, TConfItem > _ltcdacvalues;
        std::map<std::string, TConfItem > _ltcdacactivated;
        std::map<std::string, TConfItem > _ltcdacaddress;
	private:


};


#endif // __MRFDATA_LTC_H__
