/*============================================================*/
/* mrfbase.h                                                  */
/* MVD Readout Framework Base Functions                       */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFBASE_H__
#define __MRFBASE_H__

#include <sys/types.h>

//! Basic register address and content types used within MRF Suite.
namespace mrf {
	//! Register content data type.
	typedef u_int32_t registertype;
	//! Register address data type.
	typedef u_int32_t addresstype;
//	typedef u_int32_t devicetype;
}

#endif // __MRFBASE_H__

