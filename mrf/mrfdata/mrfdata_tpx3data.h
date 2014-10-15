/*============================================================*/
/* mrfdata_tpxpixel.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPX3DATA_H__
#define __MRFDATA_TPX3DATA_H__

#include "mrfdataadv1d.h"

class TMrfData_Tpx3Data : virtual public TMrfDataAdv1D
{
	public:
                TMrfData_Tpx3Data(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);

		//From TMrfDataAdvBase
		virtual void initMaps();

	protected:

	private:

};

#endif /* __MRFDATA_TPXDATA_H__ */
