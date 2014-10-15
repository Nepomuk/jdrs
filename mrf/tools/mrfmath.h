/*============================================================*/
/* mrfmath.h                                                  */
/* Mathematical Toolbox                                       */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFMATH_H__
#define __MRFMATH_H__

#include <map>
#include <sys/types.h>

namespace mrfmath {

double threshold(const std::map<u_int32_t, u_int32_t>& response, const u_int32_t& iterations);

double noise(const std::map<u_int32_t, u_int32_t>& response, const u_int32_t& iterations);

}

#endif // __MRFMATH_H__
