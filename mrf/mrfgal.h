/*============================================================*/
/* mrfgal.h                                                   */
/* MVD Readout Framework Generic Access Layer                 */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFGAL_H__
#define __MRFGAL_H__

#include <sys/types.h>
#include <poll.h>
#include "mrfbase.h"

//! Error flags set by functions accessing the SIS 1100 interface.
namespace mrf_error {
	static const u_int32_t success = 0;
	static const u_int32_t open_failed = 1;
	static const u_int32_t close_failed = 2;
	static const u_int32_t device_not_open = 4;
	static const u_int32_t read_failed = 8;
	static const u_int32_t write_failed = 16;
	static const u_int32_t ioctl_failed = 32;
	static const u_int32_t already_open = 64;
	static const u_int32_t read_data_failed = 128;
	static const u_int32_t write_data_failed = 256;
	static const u_int32_t invalid_request = 512;
    static const u_int32_t read_device_failed = 1024;
}

class TMrfData;

//! Generic Access Layer for the Mrf.
/*!
Provides an interface to the connected hardware.
Anzahl der Bits pro Char: CHAR_BIT
Gr�sse eines Typs: sizeof(TYP)
All functions which have an error code set the internal error code to 0 after successful execution.
*/
class TMrfGal
{
	public:
		TMrfGal();
		virtual ~TMrfGal();

		//! Opens a device.
		/*!
		\param devicefile Name of the device to be opened.

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrf_error::open_failed: The device could not be opened.
		*/
		virtual void openDevice(const char* const devicefile);

		//! Closes an open device.
		/*!
		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrf_error::close_failed: The device could not be closed.
		*/
		virtual void closeDevice();

		//! Polls an open device for activity.
		/*!
		\param events Event types to wait for.
		\param timetout Time in milliseconds after which function will return even without occurrence of an event.
		*/
		virtual int pollDevice(const short& events, const int& timeout);

		//! Reads data directly from the devicefile. Gets doorbell information.
		virtual u_int32_t readDevice(const u_int32_t& bytecount, TMrfData& data) const;

		//! Returns the revents values retrieved by a previous call to pollDevice.
		/*!
		\return Type of occurred event.
		*/
		virtual const short& getRevents() const;

		//! Determines if a device open.
		/*!
		\return \b True if the device is known to be open, \b False otherwise.
		*/
		virtual bool deviceIsOpen() const;

		//! Executes an ioctl on an open device.
		/*!
		The device must be open.
		This function is overloaded for convenience.
		\param request request to pass to the ioctl command

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrf_error::device_not_open: The device is not open.
			- \b mrf_error::ioctl_failed: Ioctl failed.
		*/
		virtual int doIoctl(const int& request) const;

		//! Executes an ioctl on an open device.
		/*!
		The device must be open.
		This function is overloaded for convenience.
		\param request request to pass to the ioctl command.
		\param arg additional argument to pass to the ioctl command.

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrf_error::device_not_open: The device is not open.
			- \b mrf_error::ioctl_failed: Ioctl failed.
		*/
		virtual int doIoctl(const int& request, void* arg) const;

		//! Reads from an open device.
		/*!
		The device must be open.
		Parameter list:
		\param address address of the register to read from.
		\return Register contents. Defaults to 0 if read fails.

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrf_error::device_not_open: The device is not open.
			- \b mrf_error::read_failed: Could not read from device.

		<b>Implementation notes:</b>
		Should invoke doIoctl for ioctl.
		*/
		virtual mrf::registertype read(const mrf::addresstype& address) const = 0;

		//! Writes to an open device.
		/*!
		The device must be open.
		\param address Address of the register to write to.
		\param value Value to write.

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrf_error::device_not_open: The device is not open.
			- \b mrf_error::write_failed: Could not write to device.

		<b>Implementation notes:</b>
		Should invoke doIoctl for ioctl.
		*/
		virtual void write(const mrf::addresstype& address, const mrf::registertype& value) const = 0;

		//! Combines current register contents with value in a boolean manner (\c OR).
		/*!
		The device must be open.
		\param address Address of the register to write to.
		\param value Value to binary \c OR with the previous register content and then write back.

		<b>Implementation notes:</b>
		The standard implementation should work in most cases and uses basic calls to read() and write().
		*/
		virtual void writeOr(const mrf::addresstype& address, const mrf::registertype& value) const;

		//! Combines current register contents with value in a boolean manner (\c AND).
		/*!
		The device must be open.
		\param address Address of the register to write to.
		\param value Value to binary \c AND with the previous register content and then write back.

		<b>Implementation notes:</b>
		The standard implementation should work in most cases and uses basic calls to read() and write().
		*/
		virtual void writeAnd(const mrf::addresstype& address, const mrf::registertype& value) const;

		//! Combines current register contents with value in a boolean manner (\c XOR).
		/*!
		The device must be open.
		\param address Address of the register to write to.
		\param value Value to binary \c XOR with the previous register content and then write back.

		<b>Implementation notes:</b>
		The standard implementation should work in most cases and uses basic calls to read() and write().
		*/
		virtual void writeXor(const mrf::addresstype& address, const mrf::registertype& value) const;

		//! Sets flags in a register.
		/*!
		The device must be open.
		\param address Address of the register to write to.
		\param mask Bitmask to determine which bits (flags) are affected by this function.
		\param state Determines if the chosen flags will be set (\b True) or reset (\b False).

		<b>Implementation notes:</b>
		The standard implementation should work in most cases and uses basic calls to writeAnd() and writeOr().
		*/
		virtual void setFlag(const mrf::addresstype& address, const mrf::registertype& mask, const bool& state) const;

		//! Determines if a specific flag is set.
		/*!
		The device must be open.
		If more than one bit of the bitmask is set to '1', this corresponds to a boolean OR of all tested flags.
		\param address Address of the register to read from.
		\param mask Bitmask to determine which bits (flags) are affected by this function.
		\return \b True if at least one of the tested flags is set, \b False if none of the tested flags is set.

		<b>Implementation notes:</b>
		The standard implementation should work in most cases and uses basic calls to read().
		*/
		virtual bool getFlag(const mrf::addresstype& address, const mrf::registertype& mask) const;

		//virtual void doOneShotIoctl
		//virtual void doOneShotIoctl

		//! Writes an arbitrary amount of data to an open device.
		/*!
		The device must be open.
		Parameter list:
		\param address Address of the register to write to.
		\param data TMrfData structure storing the data to be written.
		\return Amount of written data.

		<b>Error codes:</b>

		<b>Implementation notes:</b>
		*/
		virtual u_int32_t writeData(const mrf::addresstype& address, const TMrfData& data) const = 0;

		//! Reads an arbitrary amount of data from an open device.
		/*!
		The device must be open.
		\param address: Address of the register to read from.
		\param data: TMrfData structure which provides space for the data to be read.
		\return Amount of read data.

		<b>Error codes:</b>

		<b>Implementation notes:</b>
		*/
		virtual u_int32_t readData(const mrf::addresstype& address, TMrfData& data) const = 0;

		//virtual void writeData(const mrf::devicetype index, const mrf::addresstype address, const TMrfData& data) = 0;
		//virtual void readData(const mrf::devicetype index, const mrf::addresstype address, TMrfData& data) = 0;
		//virtual const u_int32_t getLastWritten() = 0;
		//virtual const u_int32_t getLastRead() = 0;

		//! Returns the internally stored error code.
		/*!
		The internal error code is a binary OR of all error flags which were set after the last successful command, which resets the error code to 0.
		\return An integer with errorflags of all errors occured after last successful command. Zero returned means success.

		<b>Implementations notes:</b>
		The standard implementation should work in most cases.
		*/
		virtual const u_int32_t& getLastError() const;

		//! Determines if the last executed member function completed successfully.
		/*!
		Details can be obtained by examining the error code returned by geLastError().
		\return \b True if the last action completed successfullly, \b False if any error occured.

		<b>Implementations notes:</b>
		The standard implementation should work in most cases.
		*/
		virtual bool lastActionSuccessful() const;

	protected:
		//! Internal error code.
		/*!
		The internal error code is a binary OR of all error flags which were set after the last successful command, which resets the error code to 0.
		*/
		mutable u_int32_t errcode;

				int p;
	private:
		//! Internal device reference.
		/*!
		The internal handle to the opened device. Only to be used by internal functions for opening, closing and accessing the device.
		*/

		short revents;
		pollfd _fds;
};

#endif // __MRFGAL_H__

