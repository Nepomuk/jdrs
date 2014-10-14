/*============================================================*/
/* mrfgal_characterdevice.cpp                                 */
/* MVD Readout Framework Generic Access Layer for Character   */
/* Based I/O								                  */
/*                                                  S.U. Esch */
/*============================================================*/

#include "mrfgal_characterdevice.h"
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>



TMrfGal_CharacterDevice::TMrfGal_CharacterDevice()
:TMrfGal(), _readbuffer("0")
{
}

void TMrfGal_CharacterDevice::openDevice(const char* const devicefile){

	//const char *device = device_file.c_str();

	if (!deviceIsOpen()) {
		p = open(devicefile, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
		if (p < 0) {
			std::cerr << "OPEN failed: " << strerror(errno) << std::endl;
			errcode |= mrf_error::open_failed;
		}
		else {
			errcode = mrf_error::success;
		}
	}
	else {
		errcode |= mrf_error::already_open;
	}
}

void TMrfGal_CharacterDevice::write( const std::string &data) {

	if (deviceIsOpen()) {

		if (::write(p, data.c_str(), data.length() ) < 0) {
			errcode |= mrf_error::write_failed;
		} else {
			errcode = mrf_error::success;
		}
	} else {
		errcode |= mrf_error::device_not_open;
	}
}

const std::string& TMrfGal_CharacterDevice::read(int read_size){

	_readbuffer.clear();
	int total_chars_read=0;
	int chars_read=0;

	if (deviceIsOpen()) {

		do{
			if (read_size-total_chars_read<MAX_CHARS_TO_READ){
				chars_read = ::read(p,buffer,read_size-total_chars_read);
				if (chars_read>0){
					_readbuffer.append(buffer, chars_read);
					total_chars_read+=chars_read;
				}
			}
			else{
				chars_read = ::read(p,buffer,MAX_CHARS_TO_READ);
				if ( chars_read >0){
					_readbuffer.append(buffer, chars_read);
					total_chars_read+=chars_read;
				}
			}
		} while (chars_read>0 && (total_chars_read <read_size));

		if (total_chars_read == 0) {
			errcode |= mrf_error::read_failed;
			std::cerr << "READ failed: " << strerror(errno) << std::endl;
			std::cerr << "READ failed: Maybe no data available? " << std::endl;
		} else {
			errcode = mrf_error::success;
		}
	} else {
		errcode |= mrf_error::device_not_open;
	}

	return _readbuffer;

}

mrf::registertype TMrfGal_CharacterDevice::read(const mrf::addresstype& ) const
{
	std::cerr << "This read command is not suitable for a character device. Please use read(std::string data)" << std::endl;
	errcode |=mrf_error::read_failed;
	return 0;
}

void TMrfGal_CharacterDevice::write(const mrf::addresstype& , const mrf::registertype&) const
{
	std::cerr << "This write command is not suitable for a character device. Please use write(std::string data)" << std::endl;
	errcode |=mrf_error::write_failed;
}

u_int32_t TMrfGal_CharacterDevice::writeData(const mrf::addresstype& , const TMrfData&) const
{
	std::cerr << "This write command is not suitable for a character device. Please use write(std::string data)" << std::endl;
	errcode |=mrf_error::write_failed;
	return 0;
}

u_int32_t TMrfGal_CharacterDevice::readData(const mrf::addresstype&, TMrfData&) const
{
	std::cerr << "This read command is not suitable for a character device. Please use read(std::string data) " << std::endl;
	errcode |=mrf_error::read_failed;
	return 0;
}
