/*============================================================*/
/* mrfdata_tpxpixel.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPXPIXEL_H__
#define __MRFDATA_TPXPIXEL_H__

#include "mrfdataadv1d.h"
#include <map>
#include <vector>

namespace topix {
	static const unsigned int colcount = 4;
	static const unsigned int longrowcount = 128;
	static const unsigned int shortrowcount = 32;
	static const unsigned int ptype = 5; // 11..7 = 0101
	static const unsigned int ntype = 10; // 11..7 = 1010
}

class TMrfData_TpxPixel : virtual public TMrfDataAdv1D
{
	public:
		TMrfData_TpxPixel(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);

		//From TMrfDataAdvBase
		virtual void initMaps();
		virtual void setPixConf(const unsigned int& col, const unsigned int& row, const std::string& item, const u_int32_t& value);
		virtual const u_int32_t& getPixConf(const unsigned int& col, const unsigned int& row, const std::string& item) const;
		virtual void assembleCol(const unsigned int& col);
		virtual const unsigned int& getNumCols() const;
		virtual const unsigned int& getNumRows(const unsigned int& col) const;
		virtual TMrfDataAdv1D::itemMap& getPixel(const unsigned int& col, const unsigned int& row);

	protected:

	private:
		unsigned int _colcount;
		std::vector<unsigned int> _rowcount;
		std::vector< std::vector<TMrfDataAdv1D::itemMap> > _pixregs;

};

#endif /* __MRFDATA_TPXPIXEL_H__ */
