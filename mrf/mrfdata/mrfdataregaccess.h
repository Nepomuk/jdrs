/*============================================================*/
/* mrfdataregaccess.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides access RB Data Registers           .              */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATAREGACCESS_H__
#define __MRFDATAREGACCESS_H__

#include "mrfdataadv1d.h"
#include <string>


class TMrfDataRegAccess : virtual public TMrfDataAdv1D
{
	public:
		TMrfDataRegAccess(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);
		virtual ~TMrfDataRegAccess();

		virtual void setValue(const std::string& targetreg, const u_int32_t& value);
		virtual u_int32_t getValue(const std::string& targetreg) const;
		virtual void setAddress(const std::string& targetreg, const u_int32_t& address);
		virtual u_int32_t getAddress(const std::string& targetreg) const;

		//From TMrfDataAdvBase
		//virtual void initMaps() = 0;

	protected:

	private:

};


#endif // __MRFDATAREGACCESS_H__
