/*============================================================*/
/* mrftal.h                                                   */
/* MVD Readout Framework Transport Access Layer               */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFTAL_H__
#define __MRFTAL_H__

#include "mrfgal.h"
#include "mrfbase.h"

//! Transport access layer for the Mrf.
/*!
Provides an interface to send data to the frontend via a transport device.
*/
class TMrfTal : virtual public TMrfGal
{
	public:
		TMrfTal();

		//! Writes an arbitrary data stream to the frontend
		/*!
		The device must be open.
		\param data TMrfData structure storing the data to be written

		<b>Error codes:</b>

		<b>Implementation notes:</b>
		*/
		virtual void writeRemoteData(const TMrfData& data) const = 0;

		//! Reads an arbitrary data stream from the frontend
		/*!
		The device must be open.
		\param data TMrfData structure which provides space for the data to be read
		
		<b>Error codes:</b>

		<b>Implementation notes:</b>
		*/
		virtual void readRemoteData(TMrfData& data) const = 0;
	protected:
		
	private:
		
};


#endif // __MRFTAL_H__

