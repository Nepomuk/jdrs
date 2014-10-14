/*============================================================*/
/* mrfdata_tpxflags.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Control Flags.                    */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPX3FLAGS_H__
#define __MRFDATA_TPX3FLAGS_H__

#include "mrfdataadv1d.h"

class TMrfData_Tpx3Flags : virtual public TMrfDataAdv1D
{
	public:
                TMrfData_Tpx3Flags(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);

		//From TMrfDataAdvBase
		virtual void initMaps();

	protected:

	private:

};

#endif /* MRFDATA_TPXFLAGS_H_ */
