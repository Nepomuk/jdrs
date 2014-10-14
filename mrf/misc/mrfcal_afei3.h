/*============================================================*/
/* mrfcal_afei3.h                                             */
/* MVD Readout Framework Chip Access Layer                    */
/* Chip Access Layer for Atlas Frontend I3                    */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFCAL_AFEI3_H__
#define __MRFCAL_AFEI3_H__

#include "mrfcal.h"
#include "mrftal_rb_sis.h"
#include "mrfbase.h"
#include "mrfdata_afei3.h"
#include <string>

//! Chip access layer (Atlas FE-I3 specific).
/*!
Provides an interface to communicate with the Atlas Frontend I3.
*/
class TMrfCal_AFEI3 : virtual public TMrfCal, virtual public TMrfTal_RB
{
	public:
		TMrfCal_AFEI3();

		//From TMrfCal
		virtual void writeToChip(const TMrfData& data) const;
		virtual void readFromChip(TMrfData& data);

		//In TMrfCal_AFEI3

		//! Configures the FE global register.
		/*!
		Updates the FE global register with the values stored in data.
		\param data
		*/
		virtual void configureGlobReg(const TMrfData_AFEI3& data) const;

		//! Configures the FE pixel registers.
		/*!
		Updates the FE pixel registers with the values stored in data. Updates all pixel register values in every activated double column. Information about active double columns is retrieved from the corresponding global register values stored in data.
		Warning: This function assumes that the global register configuration stored in data and the actual configuration of the FE's global register match.
		\param data
		*/
		virtual void configurePixReg(const TMrfData_AFEI3& data) const;

		//! Shifts the binary data stream included in data into the pixel shift register. Does not do any checks for length or enabled pixel double columns.
		virtual void shiftIntoPixReg(const TMrfData_AFEI3& data) const;
		
		//! Assembles a binary data stream from item and shifts it into the pixel shift register.
		virtual void shiftItemIntoPixReg(const TMrfData_AFEI3& data, const std::string& item) const;
		
		//! Latches the pixel register with the given value.
		virtual void latchPixReg(const mrf::registertype& address, const mrf::registertype& broadcast, const std::string& item) const;
		
		//! Reads the pixel register data given by item into the pixel shift register.
		virtual void readPixReg(const mrf::registertype& address, const mrf::registertype& broadcast, const std::string& item) const;
		
		//! Shifts the binary data stream into the pixel shift register, then latches the pixel register.
		virtual void insertPixRegData(const TMrfData_AFEI3& data, const std::string& item) const;

		//! Configures one specific item of the FE pixel register.
		virtual void configurePixRegItem(const TMrfData_AFEI3& data, const std::string& item) const;

		//! Shifts the binary data stream included in data into the global shift register. Does not do any length checks.
		virtual void shiftIntoGlobReg(const TMrfData_AFEI3& data) const;
		
		//! Latches the global register.
		virtual void latchGlobReg(const mrf::registertype& address, const mrf::registertype& broadcast) const;

		//! Reads the global register data into the global shift register.
		virtual void readGlobReg(const mrf::registertype& address, const mrf::registertype& broadcast) const;
		
		//! Configures the FE global and pixel registers.
		/*!
		\param data
		*/
		virtual void configureFE(const TMrfData_AFEI3& data) const;
	
	protected:
		
	private:
		
};


#endif // __MRFCAL_AFEI3_H__

