/*============================================================*/
/* mrfdataadvbase.h                                           */
/* MVD Readout Framework Data Storage                         */
/* Provides advanced MRF data base class.                     */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATAADVBASE_H__
#define __MRFDATAADVBASE_H__

#include "mrfdata.h"
#include "mrf_confitem.h"

class TMrfDataAdvBase : virtual public TMrfData
{
	public:
		TMrfDataAdvBase(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);
		//TMrfDataAdvBase(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse);
		virtual ~TMrfDataAdvBase();

		//virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item, const u_int32_t& index = 0, const u_int32_t& streamoffset  = 0, const u_int32_t& valueoffset = 0, const bool& reverse = false, const bool& streamreverse = false) const;
		//virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index = 0, const u_int32_t& streamoffset = 0, const u_int32_t& valueoffset = 0, const bool& reverse = false, const bool& streamreverse = false);

		virtual void setLocalConfItemValue(TConfItem& item);
		virtual void setLocalConfItemValue(TConfItem& item, const u_int32_t& value);

		virtual void setStreamConfItemValue(const TConfItem& item);

		virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item) const;
		virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value);

		virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item, const u_int32_t& index) const;
		virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index);

		virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset) const;
		virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset);

		virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset) const;
		virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset);

		virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse) const;
		virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse);

		virtual const u_int32_t& getStreamConfItemValue(const TConfItem& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const;
		virtual void setStreamConfItemValue(const TConfItem& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse);

		virtual void initMaps() = 0;

	protected:

	//private:
		u_int32_t _datastreamlength;
		u_int32_t _blocklength;
		u_int32_t _defaultindex;
		u_int32_t _defaultstreamoffset;
		u_int32_t _defaultvalueoffset;
		bool _defaultreverse;
		bool _defaultstreamreverse;
};

#endif // __MRFDATAADVBASE_H__
