/*============================================================*/
/* mrfgal_serialport.h                                                   */
/* MVD Readout Framework Generic Access Layer                 */
/* Access Layer for serial port								  */
/*                                                  S.U. Esch */
/*============================================================*/

#include "mrfgal_serialport.h"
#include <fcntl.h>

TMrfGal_SerialPort::TMrfGal_SerialPort()
:TMrfGal(), TMrfGal_CharacterDevice()
{
}

void TMrfGal_SerialPort::setDeviceConfiguration(mrf_serialport_config::BaudRate baudrate, mrf_serialport_config::Parity paritybit, mrf_serialport_config::CharacterSize wordlength)
{
	struct termios options;

	if (deviceIsOpen()) {

		fcntl(p, F_SETFL, FNDELAY);                 	 	/* Configure port reading */
															/* Get the current options for the port */
		tcgetattr(p, &options);
		cfsetispeed(&options, baudrate);
		cfsetospeed(&options, baudrate);
		                                   	   	   	   	    /* Enable the receiver and set local mode */
		options.c_cflag |= (CLOCAL | CREAD);
//		options.c_cflag &= ~PARENB; 						/* Mask the character size to 8 bits, no parity */
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE;
		options.c_cflag |=  wordlength;
		// options.c_cflag &= ~CRTSCTS;               		/* Disable hardware flow control */

		   switch (paritybit)
		    {
		    case mrf_serialport_config::PARITY_EVEN:
		    	options.c_cflag |= PARENB;
		    	options.c_cflag &= ~PARODD;
		        break;
		    case mrf_serialport_config::PARITY_ODD:
		    	options.c_cflag |= ( PARENB | PARODD );
		        break;
		    case mrf_serialport_config::PARITY_NONE:
		    	options.c_cflag &= ~(PARENB);
		        break;
		    default:
				errcode |= mrf_error::invalid_request;
		        break;
		    }

		   /* Enable data to be processed as raw input */
	//	 options.c_lflag &= ~(ICANON | ECHO | ISIG);

		                                        		/* Set the new options for the port */
		 if (tcsetattr(p, TCSANOW, &options) <0) {
			 errcode |= mrf_error::ioctl_failed;
		 }

		 errcode = mrf_error::success;
		}
	else {
		errcode |= mrf_error::device_not_open;
	}

}
