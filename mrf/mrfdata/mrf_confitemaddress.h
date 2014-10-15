/*============================================================*/
/* mrf_confitem.h                                             */
/* MVD Readout Framework Configuration Item with Address      */
/*                                               S.Esch		  */
/*============================================================*/

#ifndef MRF_CONFITEMADDRESS_H_
#define MRF_CONFITEMADDRESS_H_

#include "mrf_confitem.h"

class TConfItemAddress: public virtual TConfItem {
public:
	TConfItemAddress();
	virtual ~TConfItemAddress();

	//! Standard constructor with initialization list.
	/*!
	\param value Value to be set.
	\param position Position of the value in the data stream.
	\param length Length of the storage space reserved in the data stream.
	\param min Optional lower bounds for value.
	\param max Optional upper bounds for value.
	\param flags Optional flag field.
	\param address to which the stream should be written
	*/

	TConfItemAddress(const mrf::registertype& Value, const u_int32_t& Position, const u_int32_t& Length, const u_int32_t& Flags = 0, const u_int32_t& Address = 0);

	TConfItemAddress(const mrf::registertype& Value, const u_int32_t& Position, const u_int32_t& Length, const u_int32_t& Min, const u_int32_t& Max, const u_int32_t& Flags = 0, const u_int32_t& Address = 0);


	//! Holds the address of the data value in the data stream.
	u_int32_t address;

protected:
private:

};

#endif /* MRF_CONFITEMADDRESS_H_ */
