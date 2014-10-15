/*============================================================*/
/* mrfdata_dcmconf.h                                          */
/* MVD Readout Framework Data Storage                         */
/* Specific to Virtex4 DCM_ADV Data Format                    */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_DCMCONF_H__
#define __MRFDATA_DCMCONF_H__

#include <map>
#include <string>
#include "mrfdata.h"
#include "mrf_confitem.h"

class TMrfData_DCMConf : virtual public TMrfData
{
	public:
		TMrfData_DCMConf();

		u_int32_t getItemValue(const std::string& source, const std::string& item) const;
		void setItemValue(const std::string& source, const std::string& item, const mrf::registertype& value);

		void assemble(const std::string& source);

		std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator getTypeIteratorBegin() const;
		std::map<const std::string, std::map<const std::string, TConfItem> >::const_iterator getTypeIteratorEnd() const;
		std::map<const std::string, TConfItem>::const_iterator getItemIteratorBegin(const std::string& type) const;
		std::map<const std::string, TConfItem>::const_iterator getItemIteratorEnd(const std::string& type) const;


	protected:

		void initMaps();
		void initMultiplierStructure();
		void initDividerStructure();

		std::map<const std::string, std::map<const std::string, TConfItem> > maplist;
};

#endif // __MRFDATA_DCMCONF_H__

