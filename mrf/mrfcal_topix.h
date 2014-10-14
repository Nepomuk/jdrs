/*============================================================*/
/* mrfcal_topix.h                                             */
/* MVD Readout Framework Chip Access Layer                    */
/* Chip Access Layer for Topix Frontend Prototype             */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFCAL_TOPIX_H__
#define __MRFCAL_TOPIX_H__

#include "mrftal_rbtpx.h"
#include "mrfcal.h"
#include "mrfdata/mrfdata_ltc2620.h"
#include "mrfdata/mrfdata_tpxmasterreg.h"

class TMrfCal_Topix : virtual public TMrfCal, virtual public TMrfTal_RBTopix
{
	public:
		TMrfCal_Topix();
		virtual ~TMrfCal_Topix();
		
                virtual void configLTC(TMrfData_LTC2620 data) const;
		virtual void prepareMaster(TMrfData_TpxMasterReg data) const;

		//From TMrf_Cal
		virtual void writeToChip(const TMrfData& data) const;
		virtual void readFromChip(TMrfData& data);

	protected:
	private:
};

#endif /* __MRFCAL_TOPIX_H__ */
