/*============================================================*/
/* mrfcal.h                                                   */
/* MVD Readout Framework Chip Access Layer                    */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFCAL_H__
#define __MRFCAL_H__

#include "mrftal.h"
#include "mrfbase.h"

//! Chip access layer for the Mrf.
/*!
Provides an interface to send data to the frontend chip.
*/
class TMrfCal : virtual public TMrfTal
{
	public:
		TMrfCal();

		//! Writes an arbitrary data stream to the frontend.
		/*!
		The device must be open. The data sent is the binary data stored within the structure. Assembly of high level data has to be done by the user.
		\param data TMrfData structure storing the data to be written.
		<b>Error codes:</b>

		<b>Implementation notes:</b>
		*/
		virtual void writeToChip(const TMrfData& data) const = 0;

		//! Reads an arbitrary data stream from the frontend.
		/*!
		The device must be open. The data read is stored as binary data within the TMrfData structure. Disassembly to obtain high level values has to be done by the user.
		\param data TMrfData structure which provides space for the data to be read.
		<b>Error codes:</b>

		<b>Implementation notes:</b>
		*/
		virtual void readFromChip(TMrfData& data) = 0;
	protected:
		
	private:
		
};




#endif // __MRFCAL_H__

