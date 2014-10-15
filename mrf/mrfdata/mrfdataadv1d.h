/*============================================================*/
/* mrfdataadv1d.h                                             */
/* MVD Readout Framework Data Storage                         */
/* Provides generic 1D TConfItem map and accessors.           */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATAADV1D_H__
#define __MRFDATAADV1D_H__

#include "mrfdataadvbase.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

class TMrfDataAdv1D : virtual public TMrfDataAdvBase
{
	public:
		typedef std::map<std::string, TConfItem> itemMap;
		typedef std::map<std::string, TConfItem>::const_iterator constItemIterator;
		typedef std::map<std::string, TConfItem>::iterator itemIterator;
	
		//TMrfDataAdv1D(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);
		TMrfDataAdv1D(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse);
		virtual ~TMrfDataAdv1D();

		virtual const u_int32_t& getLocalItemValue(const std::string& item) const;
		virtual void setLocalItemValue(const std::string& item, const u_int32_t& value);
		//virtual const u_int32_t& getStreamItemValue(const std::string& item, const u_int32_t& index = _defaultindex, const u_int32_t& streamoffset = _defaultstreamoffset, const u_int32_t& valueoffset = _defaultvalueoffset, const bool& reverse = _defaultreverse, const bool& streamreverse = _defaultstreamreverse) const;
		//virtual void setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index = _defaultindex, const u_int32_t& streamoffset = _defaultstreamoffset, const u_int32_t& valueoffset = _defaultvalueoffset, const bool& reverse = _defaultreverse, const bool& streamreverse = _defaultstreamreverse);
		virtual void setStreamItemValue(const std::string& item);
		virtual const u_int32_t& getStreamItemValue(const std::string& item) const;
		virtual void setStreamItemValue(const std::string& item, const u_int32_t& value);
		virtual const u_int32_t& getStreamItemValue(const std::string& item, const u_int32_t& index) const;
		virtual void setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index);
		virtual const u_int32_t& getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset) const;
		virtual void setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset);
		virtual const u_int32_t& getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset) const;
		virtual void setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset);
		virtual const u_int32_t& getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse) const;
		virtual void setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse);
		virtual const u_int32_t& getStreamItemValue(const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const;
		virtual void setStreamItemValue(const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse);

		virtual void assemble();
		virtual void disassemble();
		
		virtual int getItemCount() const;

		virtual constItemIterator getConstItemIteratorBegin() const;
		virtual constItemIterator getConstItemIteratorEnd() const;
		virtual itemIterator getItemIteratorBegin();
		virtual itemIterator getItemIteratorEnd();
		
	protected:
		std::map<std::string, TConfItem > _localdata;

	private:

};


#endif // __MRFDATAADV1D_H__
