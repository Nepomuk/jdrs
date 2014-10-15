/*============================================================*/
/* mrfgal.cpp                                                 */
/* MVD Readout Framework Generic Access Layer                 */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrfgal.h"
#include "mrfdata.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>
#include <climits>
#include <iostream>
#include <string.h>


TMrfGal::TMrfGal()
: errcode(0), p(-1), revents(0)
{
	//fds = new(pollfd);
}

TMrfGal::~TMrfGal()
{
	closeDevice();
	//delete(fds);
}

void TMrfGal::openDevice(const char* const devicefile)
{
	if (!deviceIsOpen()) {
		p = open(devicefile, O_RDWR, 0);
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

void TMrfGal::closeDevice()
{
	if (deviceIsOpen()) {
		if (close(p) == 0) {
			p = -1;
			errcode = mrf_error::success;
		}
		else {
			p = -1;
			std::cerr << "CLOSE failed: " << strerror(errno) << std::endl;
			errcode |= mrf_error::close_failed;
		}
	}
	else {
		errcode |= mrf_error::device_not_open;
	}
}

int TMrfGal::pollDevice(const short& events, const int& timeout)
{
	//int rv;
	if (deviceIsOpen()) {
		_fds.fd = p;
		_fds.events = events;
		int rv = poll(&_fds, 1, timeout);
		revents = _fds.revents;
		return rv;
	} else {
		errcode |= mrf_error::device_not_open;
		return -1;
	}
}

u_int32_t TMrfGal::readDevice(const u_int32_t& bytecount, TMrfData& data) const
{
	if (deviceIsOpen()) {
		u_int32_t wordcount = (bytecount + (sizeof(char) * CHAR_BIT) - 1) / (sizeof(char) * CHAR_BIT);
		u_int32_t* readbuf = new u_int32_t[wordcount];
		int bytes_read = ::read(p, readbuf, bytecount);
		data.clearDataStream();
		if (bytes_read < 0) {
			delete[] readbuf;
			errcode |= mrf_error::read_device_failed;
			return 0;
		} else {
			u_int32_t words_read = (bytes_read + (sizeof(char) * CHAR_BIT) - 1) / (sizeof(char) * CHAR_BIT);
			data.setNumWords(words_read);
			for (unsigned int i = 0; i < words_read; ++i) {
				data.setWord(i, readbuf[i], false);
			}
			delete[] readbuf;
			data.setNumBits(bytes_read * sizeof(char) * CHAR_BIT);
			errcode = 0;
			return bytes_read;
		}
	} else {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
}

const short& TMrfGal::getRevents() const
{
	return revents;
}

bool TMrfGal::deviceIsOpen() const
{
	return (p != -1);
}

int TMrfGal::doIoctl(const int& request) const
{
	if (!deviceIsOpen()) {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
	else {
		int tmp = ioctl(p, request);
		if (tmp < 0) {
			std::cerr << "IOCTL failed: " << strerror(errno) << std::endl;
			errcode |= mrf_error::ioctl_failed;
		}
		else {
			errcode = mrf_error::success;
		}
		return tmp;
	}
}

int TMrfGal::doIoctl(const int& request, void* arg) const
{
	if (!deviceIsOpen()) {
		errcode |= mrf_error::device_not_open;
		return 0;
	}
	else {
		int tmp = ioctl(p, request, arg);
		if (tmp < 0) {
			std::cerr << "IOCTL failed: " << strerror(errno) << std::endl;
			errcode |= mrf_error::ioctl_failed;
		}
		else {
			errcode = mrf_error::success;
		}
		return tmp;
	}
}

void TMrfGal::writeOr(const mrf::addresstype& address, const mrf::registertype& value) const
{
	write(address, read(address) | value);
}

void TMrfGal::writeAnd(const mrf::addresstype& address, const mrf::registertype& value) const
{
	write(address, read(address) & value);
}

void TMrfGal::writeXor(const mrf::addresstype& address, const mrf::registertype& value) const
{
	write(address, read(address) ^ value);
}

void TMrfGal::setFlag(const mrf::addresstype& address, const mrf::registertype& mask, const bool& state) const
{
	if (state) {
		writeOr(address, mask);
	}
	else {
		writeAnd(address, ~mask);
	}
}

bool TMrfGal::getFlag(const mrf::addresstype& address, const mrf::registertype& mask) const
{
	return bool (read(address) & mask);
}

const u_int32_t& TMrfGal::getLastError() const
{
	return errcode;
}

bool TMrfGal::lastActionSuccessful() const
{
	return (errcode == 0);
}

