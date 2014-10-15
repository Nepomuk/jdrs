/*============================================================*/
/* mrfdata_ltc2620.h                                          */
/* MVD Readout Framework Data Storage                         */
/* Provides access to LTC2620 DAC configuration.              */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_LTC2604_H__
#define __MRFDATA_LTC2604_H__

#include "mrfdata_ltc.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

class TMrfData_LTC2604 : virtual public TMrfData_LTC
{
	public:
        TMrfData_LTC2604(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);

        virtual void initMaps();

	protected:

	private:
};


#endif // __MRFDATA_LTC2604_H__
