/*============================================================*/
/* mrfdataadv2d.h                                             */
/* MVD Readout Framework Data Storage                         */
/* Provides generic 2D TConfItem map and accessors.           */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATAADV2D_H__
#define __MRFDATAADV2D_H__

#include "mrfdataadvbase.h"
#include <map>
#include <string>
#include "mrf_confitem.h"

class TMrfDataAdv2D : virtual public TMrfDataAdvBase
{
	public:
		typedef std::map<std::string, TConfItem>::const_iterator constItemIterator;
		typedef std::map<std::string, TConfItem>::iterator itemIterator;
		typedef std::map<std::string, std::map<std::string, TConfItem> >::const_iterator constTypeIterator;
		typedef std::map<std::string, std::map<std::string, TConfItem> >::iterator typeIterator;
	
		TMrfDataAdv2D(const u_int32_t& blocklength = bitsinablock);
		virtual ~TMrfDataAdv2D();

		virtual const u_int32_t& getLocalItemValue(const std::string& source, const std::string& item) const;
		virtual void setLocalItemValue(const std::string& source, const std::string& item, const u_int32_t& value);
		//virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index = _defaultindex, const u_int32_t& streamoffset = _defaultstreamoffset, const u_int32_t& valueoffset = _defaultvalueoffset, const bool& reverse = _defaultreverse, const bool& streamreverse = _defaultstreamreverse) const;
		//virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index = _defaultindex, const u_int32_t& streamoffset = _defaultstreamoffset, const u_int32_t& valueoffset = _defaultvalueoffset, const bool& reverse = _defaultreverse, const bool& streamreverse = _defaultstreamreverse);

		virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item) const;
		virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value);
                virtual void setStreamItemValue(const std::string& source, const std::string& item);
		virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index) const;
		virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index);
		virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset) const;
		virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset);
		virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset) const;
		virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset);
		virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse) const;
		virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse);
		virtual const u_int32_t& getStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse) const;
		virtual void setStreamItemValue(const std::string& source, const std::string& item, const u_int32_t& value, const u_int32_t& index, const u_int32_t& streamoffset, const u_int32_t& valueoffset, const bool& reverse, const bool& streamreverse);

		virtual void assemble();
		virtual void disassemble();
		
		virtual int getTypeCount() const;
		virtual int getItemCount(const std::string& type) const;

		virtual constTypeIterator getConstTypeIteratorBegin() const;
		virtual constTypeIterator getConstTypeIteratorEnd() const;
		virtual constItemIterator getConstItemIteratorBegin(const std::string& type) const;
		virtual constItemIterator getConstItemIteratorEnd(const std::string& type) const;
		virtual constItemIterator getConstItemIteratorBegin(const std::map<std::string, std::map<std::string, TConfItem> >::const_iterator& type) const;
		virtual constItemIterator getConstItemIteratorEnd(const std::map<std::string, std::map<std::string, TConfItem> >::const_iterator& type) const;

                virtual typeIterator getTypeIteratorBegin();
                virtual typeIterator getTypeIteratorEnd();

	protected:

		virtual itemIterator getItemIteratorBegin(const std::string& type);
		virtual itemIterator getItemIteratorEnd(const std::string& type);
		virtual itemIterator getItemIteratorBegin(const std::map<std::string, std::map<std::string, TConfItem> >::iterator& type);
		virtual itemIterator getItemIteratorEnd(const std::map<std::string, std::map<std::string, TConfItem> >::iterator& type);

		std::map<std::string, std::map<std::string, TConfItem> > _localdata;

	private:

};

#endif // __MRFDATAADV2D_H__
