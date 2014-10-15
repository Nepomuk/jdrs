/*============================================================*/
/* mrfdata_afei3hits.h                                        */
/* MVD Readout Framework Data Storage                         */
/* Specific to Atlas FE-I3 Hit Word Format                    */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_AFEI3HITS_H__
#define __MRFDATA_AFEI3HITS_H__

#include <map>
#include <string>
#include "mrfdata.h"
#include "mrf_confitem.h"

class TMrfData_AFEI3Hits : virtual public TMrfData
{
	public:
		TMrfData_AFEI3Hits();

		const u_int32_t& getItemValue(const std::string& source, const std::string& item, const u_int32_t& index) const;
		const std::string& getDataType(const u_int32_t& index, const u_int32_t& parity) const;
		bool isEOEWord(const u_int32_t& index) const;
		bool isHitWord(const u_int32_t& index) const;
		const std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator getTypeIteratorBegin() const;
		const std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator getTypeIteratorEnd() const;
		const std::map<const std::string, TConfItem>::const_iterator getItemIteratorBegin(const std::string& type) const;
		const std::map<const std::string, TConfItem>::const_iterator getItemIteratorEnd(const std::string& type) const;

	protected:
		void initMaps();
		void initEOEStructure();
		void initEOEPStructure();
		void initHitStructure();
		void initHitPStructure();

		std::map<const std::string, std::map<const std::string, TConfItem> > maplist;

	private:
		static const std::string _eoep;
		static const std::string _hitp;
		static const std::string _eoe;
		static const std::string _hit;
		
		/*
		std::map<const std::string, TConfItem> hitword;
		std::map<const std::string, TConfItem> eoeword;
		std::map<const std::string, TConfItem> hitwordp;
		std::map<const std::string, TConfItem> eoewordp;
		*/
};

#endif // __MRFDATA_AFEI3HITS_H__

