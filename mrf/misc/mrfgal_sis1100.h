/*============================================================*/
/* mrfgal_sis1100.h                                           */
/* MVD Readout Framework Generic Access Layer                 */
/* Generic Access Layer for SIS1100 based devices.            */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFGAL_SIS1100_H__
#define __MRFGAL_SIS1100_H__

#include "mrfgal.h"
#include "sis1100_var.h"
#include "mrfbase.h"

//void* irqhelper(void* ptr);

//! Identifiers to access the different values stored in a sis1100_ident structure.
namespace sis_ident {
	//! Identifiers to access the different values stored in a sis1100_ident structure.
	enum ident_detail {
		localhwtype,
		localhwversion,
		localfwtype,
		localfwversion,
		remotehwtype,
		remotehwversion,
		remotefwtype,
		remotefwversion,
		remoteok,
		remoteonline
	};
}

//! Magic numbers used by SIS 1100 interface
namespace sis_magic {
	static const int irqctlsignal_on = -1;
	static const int irqctlsignal_off = 0;
	static const u_int32_t doorbellreg = 4;
}

//! Generic Access Layer (SIS1100 specific).
/*!
Provides an interface to the SIS1100 communication device.
All functions which have an error code set the internal error code to 0 after successful execution.
*/
class TMrfGal_Sis1100 : virtual public TMrfGal
{
	public:
		TMrfGal_Sis1100();

		// From TMrfGal
		virtual mrf::registertype read(const mrf::addresstype& address) const;
		virtual void write(const mrf::addresstype& address, const mrf::registertype& value) const;
		virtual u_int32_t writeData(const mrf::addresstype& address, const TMrfData& data) const;
		virtual u_int32_t readData(const mrf::addresstype& address, TMrfData& data) const;

		// In TMrfGal_Sis1100
		//! SIS1100 IRQ Handling
		virtual void enableIRQ(const u_int32_t& mask) const;
		virtual void disableIRQ(const u_int32_t& mask) const;
		virtual u_int32_t waitforIRQ(const u_int32_t& mask);
		virtual u_int32_t getIRQ(const u_int32_t& mask);
		virtual u_int32_t waitforIRQFast(const u_int32_t& mask) const;
		virtual u_int32_t getIRQFast(const u_int32_t& mask) const;
		virtual void ackIRQFast(u_int32_t mask) const;

		//virtual const mrf::registertype& getLastDoorbell() const;
		virtual const u_int32_t& getLastInterrupt() const;

		//! Returns the identification information of an opened device.
		/*!
		The control device must be open.
		\return Complete SIS 1100 identification information stored in a sis1100_ident structure.
		*/
		sis1100_ident getFullIdent() const;

		//! Returns a specific value of the identification information of an opened device.
		/*!
		The control device must be open.
		\param detail Identification detail to retrieve. See sis_ident::ident_detail for further reference.
		\return Identification detail.
		*/
		int getFullIdentDetail(const sis_ident::ident_detail detail) const;
	protected:

	private:
		//sis1100_ctrl_reg reg;
		sis1100_irq_get irqget;
		mrf::registertype doorbell;
};

#endif // __MRFGAL_SIS1100_H__

