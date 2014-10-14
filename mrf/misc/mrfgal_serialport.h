/*============================================================*/
/* mrfgal_serialport.h                                        */
/* MVD Readout Framework Generic Access Layer                 */
/* Access Layer for serial port								  */
/*                                                  S.U. Esch */
/*============================================================*/

#ifndef MRFGAL_SERIALPORT_H_
#define MRFGAL_SERIALPORT_H_

#include "mrfgal_characterdevice.h"
#include <termios.h>

namespace mrf_serialport_config {
enum BaudRate {
			BAUD_50      = B50,    	//!< 50 Baud
			BAUD_75      = B75,    	//!< 75 Baud
			BAUD_110     = B110,	//!< 110 Baud
			BAUD_134     = B134,	//!< 134 Baud
			BAUD_150     = B150,	//!< 150 Baud
			BAUD_200     = B200,	//!< 200 Baud
			BAUD_300     = B300,	//!< 300 Baud
			BAUD_600     = B600,	//!< 600 Baud
			BAUD_1200    = B1200,	//!< 1200 Baud
			BAUD_1800    = B1800,	//!< 1800 Baud
			BAUD_2400    = B2400,	//!< 2400 Baud
        	BAUD_4800    = B4800,	//!< 4800 Baud
        	BAUD_9600    = B9600,	//!< 9600 Baud
        	BAUD_19200   = B19200,	//!< 19200 Baud
        	BAUD_38400   = B38400,	//!< 38400 Baud
        	BAUD_57600   = B57600,	//!< 57600 Baud
        	BAUD_115200  = B115200,	//!< 115200 Baud
        	BAUD_230400  = B230400,	//!< 230400 Baud
        	BAUD_DEFAULT = BAUD_9600
		};

		enum CharacterSize {
			CHAR_SIZE_5  = CS5, //!< 5 bit characters.
			CHAR_SIZE_6  = CS6, //!< 6 bit characters.
			CHAR_SIZE_7  = CS7, //!< 7 bit characters.
			CHAR_SIZE_8  = CS8, //!< 8 bit characters.
			CHAR_SIZE_DEFAULT = CHAR_SIZE_8
		};

		enum Parity {
			PARITY_EVEN,     //!< Even parity.
			PARITY_ODD,      //!< Odd parity.
			PARITY_NONE,     //!< No parity i.e. parity checking disabled.
			PARITY_DEFAULT = PARITY_NONE
		};
}
//! Generic Access Layer for Serial Port
/*! Provides an Interface for communication and configuration of Serial Port devices
 */

class TMrfGal_SerialPort: virtual public TMrfGal_CharacterDevice
{
	public:
		TMrfGal_SerialPort();



    //! Sets the configuration for the Serial Port
    /*!
	/param baudrate Sets the Baudrate for the Device. Needs to be identical with communication partner.
	/param parity Sets if a Parity Bit is used or not. Needs to be identical with communication partner.
	/param wordlength Sets the length of a word. Needs to be identical with communication partner.
     */

	void setDeviceConfiguration(mrf_serialport_config::BaudRate baudrate, mrf_serialport_config::Parity parity, mrf_serialport_config::CharacterSize wordlength);
};

#endif /* MRFGAL_SERIALPORT_H_ */
