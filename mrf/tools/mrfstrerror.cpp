#include "mrfstrerror.h"

TMrfStrError::TMrfStrError()
{
	_error[mrf_error::success] = "Success (ERR 0)";
	_error[mrf_error::open_failed] = "Open device failed! (ERR 1)";
	_error[mrf_error::close_failed] = "Closing device failed! (ERR 2)";
	_error[mrf_error::device_not_open] = "Device is not opened! (ERR 4)";
	_error[mrf_error::read_failed] = "Read failed! (ERR 8)";
	_error[mrf_error::write_failed] = "Write failed (ERR 16)";
	_error[mrf_error::ioctl_failed] = "IOCTL failed! (ERR 32)";
	_error[mrf_error::already_open] = "Device already opened! (ERR 64)";
	_error[mrf_error::read_data_failed] = "Reading data failed! (ERR 128)";
	_error[mrf_error::write_data_failed] = "Writing data failed! (ERR 256)";
	_error[mrf_error::invalid_request] = "Invalid Request! (ERR 512)";
	_error[mrf_error::read_device_failed] = "Reading device failed! (ERR 1024)";
	setDelimiter("\n");
	setSuccessString(_error[mrf_error::success]);
}

const std::string& TMrfStrError::getErrorStr(u_int32_t errornum)
{
	if (errornum == 0) return _error[mrf_error::success];
    _errorstr = "";
    for (rit=_error.rbegin(); rit != _error.rend(); ++rit ){
        if (errornum >= rit->first && errornum > 0){
            _errorstr = _errorstr + _delimiter +rit->second;
            errornum = errornum - rit->first;
        }
    }
    return _errorstr;
}

void TMrfStrError::setDelimiter(std::string delimiter){
	_delimiter = delimiter;
}

std::string TMrfStrError::getDelimiter(){
	return _delimiter;
}

void TMrfStrError::setSuccessString(std::string successstring){
	_error[mrf_error::success] = successstring;
}

std::string TMrfStrError::getSuccessString(){
	return _error[mrf_error::success];
}
