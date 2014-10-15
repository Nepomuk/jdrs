/*============================================================*/
/* mrfgal_characterdevice.h                                   */
/* MVD Readout Framework Generic Access Layer for Character   */
/* Based I/O								                  */
/*                                                  S.U. Esch */
/*============================================================*/

#ifndef MRFGAL_CHARACTERDEVICE_H_
#define MRFGAL_CHARACTERDEVICE_H_

#include "mrfgal.h"
#include <string>

#define MAX_CHARS_TO_READ 100

//! Generic Access Layer for Character Based I/O.
/*! Provides an interface for hardware with character based I/O like serial port.
All functions which have an error code set the internal error code to 0 after successful execution.
*/



class TMrfGal_CharacterDevice: virtual public TMrfGal
{
	public:
		TMrfGal_CharacterDevice();

		//! Opens a device
		/*!
		/param devicefile Name of the device to be opened.

		<b> Error codes: </b>
				- \b 0: Everything OK.
		 	 	- \b mrf_error::open_failed: The device could not be opened.
		 	 	- \b mrf_error::already_opened: The device is already open.
		 	 	*/
		void openDevice(const char* const devicefile);

		//! Writes to an open device.
		/*!
		The device must be open
		\param data Data to write to the device

		<b> Error codes: </b>
				- \b 0: Everything OK.
		 	 	- \b mrf_error::write_failed: Could not write to device.
		 	 	- \b mrf_error::device_not_opend: The device is not open.
		*/

		void write(const std::string &data);

		//! Reads from an open device.
		/*!
		The device must be open.
		\param read_size Number of characters to read from device

		<b> Error codes: </b>
				- \b 0: Everything OK.
		 	 	- \b mrf_error::read_failed: Could not read from device OR no data available for reading
		 	 	- \b mrf_error::device_not_opend: The device is not open.
		*/

		const std::string& read(int read_size=MAX_CHARS_TO_READ);

		// aus MRF_Gal

		virtual mrf::registertype read(const mrf::addresstype& address) const;
		virtual void write(const mrf::addresstype& address, const mrf::registertype& value) const;
		virtual u_int32_t writeData(const mrf::addresstype& address, const TMrfData& data) const;
		virtual u_int32_t readData(const mrf::addresstype& address, TMrfData& data) const;

	private:
		std::string _readbuffer;
		char buffer[MAX_CHARS_TO_READ];

};

#endif /* MRFGAL_CHARACTERDEVICE_H_ */
