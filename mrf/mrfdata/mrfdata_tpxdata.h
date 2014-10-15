/*============================================================*/
/* mrfdata_tpxpixel.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPXDATA_H__
#define __MRFDATA_TPXDATA_H__

#include "mrfdataadv1d.h"

class TMrfData_TpxData : virtual public TMrfDataAdv1D
{
	public:
		TMrfData_TpxData(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);

		virtual u_int32_t getHiWord(const unsigned int& index) const;
		virtual u_int32_t getLoWord(const unsigned int& index) const;
		virtual void setHiWord(const unsigned int& index, const u_int32_t& value);
		virtual void setLoWord(const unsigned int& index, const u_int32_t& value);

		//From TMrfDataAdvBase
		virtual void initMaps();

	protected:

	private:

};

#endif /* __MRFDATA_TPXDATA_H__ */
