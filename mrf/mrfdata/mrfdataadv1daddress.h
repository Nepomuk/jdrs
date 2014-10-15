/*============================================================*/
/* mrfdataadv1d.h                                             */
/* MVD Readout Framework Data Storage                         */
/* Provides generic 1D TConfItemAddress map and accessors.    */
/*                                               S.Esch		  */
/*============================================================*/

#ifndef __MRFDATAADV1DADDRESS_H__
#define __MRFDATAADV1DADDRESS_H__

#include "mrfdataadv1d.h"
#include <string>


class TMrfDataAdv1DAddress : virtual public TMrfDataAdv1D
{
	public:
	
		TMrfDataAdv1DAddress(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse);
		virtual ~TMrfDataAdv1DAddress();
		
		virtual const u_int32_t& getLocalItemAddress(const std::string& item) const;

	protected:


	private:

};


#endif // __MRFDATAADV1DADDRESS_H__
