/*============================================================*/
/* mrftal_agilent33250a.h                                     */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Agilent 33250a Pulsgenerator    */
/*                                                  S.U. Esch */
/*============================================================*/

#ifndef MRFTAL_AGILENT33250A_H_
#define MRFTAL_AGILENT33250A_H_

#include "mrfgal_serialport.h"

//! Transport Access Layer for Agilent 33250a Pulsgenerator
/*! Provides an interface for Agilent 33250a Pulsgenerator via RS232 serial port

 */

class TMrfTal_Agilent33250a: virtual public TMrfGal_SerialPort
{
	public:
		TMrfTal_Agilent33250a();

		//! Send trigger command to Pulsgenerator
		/*! Sends a trigger command to Agilent Pulsgenerator.
		 Pulsgenerator needs to be in Burst or Sweep Mode.
		 */

		void doTrigger();

		//! Send beep command to Pulsgenerator
		/*! Sends a beep command to Agilent Pulsgenerator.
		 Pulsgenerator emits immediately a beep.
		 */

		void doBeep();


		//! Opens the device
		/*! Open and configure the connection to Agilent Pulsgenerator via serial port
		 		/param devicefile Name of the device to be opened.
		 */

		virtual void openDevice(const char* const devicefile);



};

#endif /* MRFTAL_AGILENT33250A_H_ */
