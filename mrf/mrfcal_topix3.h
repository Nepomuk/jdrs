/*============================================================*/
/* mrfcal_topix.h                                             */
/* MVD Readout Framework Chip Access Layer                    */
/* Chip Access Layer for Topix3 Frontend Prototype            */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFCAL_TOPIX3_H__
#define __MRFCAL_TOPIX3_H__

#include "mrftal_rbtpx3.h"
#include "mrfcal.h"
#include "mrfdata_chain2ltc2604.h"
#include "mrfdata_tpxmasterreg.h"
#include "mrfdata_tpx3config.h"


class TMrfCal_Topix3 : virtual public TMrfCal, virtual public TMrfTal_RBTopix3
{
	public:
                TMrfCal_Topix3();
                virtual ~TMrfCal_Topix3();
		
                virtual void configLTC(TMrfData_Chain2LTC2604 data) const;
                virtual void configCCR(TMrfData_Tpx3Config) const;
                virtual void readCCR0(TMrfData_Tpx3Config& data) const;
                virtual void readCCR1(TMrfData_Tpx3Config& data) const;
                virtual void readCCR2(TMrfData_Tpx3Config& data) const;
                virtual void writeCommand(TMrfData_Tpx3Config& data) const;

		//From TMrf_Cal
		virtual void writeToChip(const TMrfData& data) const;
		virtual void readFromChip(TMrfData& data);

	protected:
	private:
};

#endif /* __MRFCAL_TOPIX3_H__ */
