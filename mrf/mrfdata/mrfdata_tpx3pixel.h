/*============================================================*/
/* mrfdata_tpxpixel.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPX3PIXEL_H__
#define __MRFDATA_TPX3PIXEL_H__

#include "mrfdataadv1d.h"

class TMrfData_Tpx3Pixel : virtual public TMrfDataAdv1D
{
	public:
                TMrfData_Tpx3Pixel(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);

		//From TMrfDataAdvBase
		virtual void initMaps();
                virtual void assemble();

                virtual void setCommand(u_int32_t command);

	protected:

	private:

};

#endif /* __MRFDATA_TPXPIXEL_H__ */
