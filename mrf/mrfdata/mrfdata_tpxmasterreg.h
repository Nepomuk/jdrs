/*============================================================*/
/* mrfdata_tpxmasterreg.h                                     */
/* MVD Readout Framework Data Storage                         */
/* Provides access to RB Topix Master Control  .              */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPXMASTERREG_H__
#define __MRFDATA_TPXMASTERREG_H__

//#include "mrfdataadv1d.h"
#include "mrfdataregaccess.h"
//#include <map>
#include <string>
//#include "mrf_confitem.h"


class TMrfData_TpxMasterReg : virtual public TMrfDataRegAccess
{
	public:
		TMrfData_TpxMasterReg(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);
		virtual ~TMrfData_TpxMasterReg();

		//From TMrfDataAdvBase
		virtual void initMaps();

	protected:

	private:

};


#endif // __MRFDATA_TPXMASTERREG_H__
