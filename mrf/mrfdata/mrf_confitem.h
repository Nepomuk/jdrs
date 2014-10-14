/*============================================================*/
/* mrf_confitem.h                                             */
/* MVD Readout Framework Configuration Item                   */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRF_CONFITEM_H__
#define __MRF_CONFITEM_H__

#include "mrfbase.h"

//! Configuration Item to be used in TMrfData structures.
/*!
Holds information about values, position and length of data items.
*/
class TConfItem
{
	public:
		TConfItem();

		//! Standard constructor with initialization list.
		/*!
		\param value Value to be set.
		\param position Position of the value in the data stream.
		\param length Length of the storage space reserved in the data stream.
		\param min Optional lower bounds for value.
		\param max Optional upper bounds for value.
		\param flags Optional flag field.
		*/
		//TConfItem(const mrf::registertype value, const u_int32_t position, const u_int32_t length);

		TConfItem(const mrf::registertype& value, const u_int32_t& position, const u_int32_t& length, const u_int32_t& flags = 0);

		TConfItem(const mrf::registertype& value, const u_int32_t& position, const u_int32_t& length, const u_int32_t& min, const u_int32_t& max, const u_int32_t& flags = 0);

		//! Holds the data value.
		mrf::registertype value;

		//! Holds the position of the data value in the data stream.
		u_int32_t position;
	
		//! Holds the length of reserved space for the data value in the data stream.
		u_int32_t length;

		//! Minimum valid value. Corresponds to \b disabled for state data.
		u_int32_t min;

		//! Maximum valid value. Corresponds to \b enabled for state data.
		u_int32_t max;

		//! Flag field to store additional properties of data.
		u_int32_t flags;
	protected:
	private:
};

#endif // __MRF_CONFITEM_H__

