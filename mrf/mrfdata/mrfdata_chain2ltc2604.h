/*============================================================*/
/* mrfdata_chain2ltc2620.h                                    */
/* MVD Readout Framework Data Storage                         */
/* Provides access to two LTC2604 DAC daisychain              */
/* configuration.                                             */
/*                                                    S. Esch */
/*============================================================*/

#ifndef __MRFDATA_CHAIN2LTC2604_H__
#define __MRFDATA_CHAIN2LTC2604_H__

#include "mrfdata_chainltc.h"
#include <map>
#include <string>
#include "mrf_confitem.h"


class TMrfData_Chain2LTC2604 : virtual public TMrfData_ChainLTC
{
	public:
        TMrfData_Chain2LTC2604(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);

        virtual void initMaps();
};


#endif // __MRFDATA_CHAIN2LTC2604_H__
