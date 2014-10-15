/*============================================================*/
/* mrfmath.cpp                                                */
/* Mathematical Toolbox                                       */
/*                                               M.C. Mertens */
/*============================================================*/

#include "mrfmath.h"
#include <cmath>

namespace mrfmath {

double threshold(const std::map<u_int32_t, u_int32_t>& response, const u_int32_t& iterations)
{
	double dv = static_cast<double>(response.rbegin()->first - response.begin()->first) / static_cast<double>(response.size());
	unsigned a = 0;
	for (std::map<u_int32_t, u_int32_t>::const_iterator iter = response.begin(); iter != response.end(); ++iter) {
		a += iter->second;
	}
	return (static_cast<double>(response.rbegin()->first) - ((static_cast<double>(a)*dv)/static_cast<double>(iterations)));
}

double noise(const std::map<u_int32_t, u_int32_t>& response, const u_int32_t& iterations)
{
	double mu = threshold(response, iterations);
	unsigned a = 0;
	for (std::map<u_int32_t, u_int32_t>::const_iterator iter = response.begin(); iter != response.end(); ++iter) {
		if (iter->first < mu) {
			a += iter->second;
		} else {
			a += (iterations - iter->second);
		}
	}
	return static_cast<double>((response.rbegin()->first - response.begin()->first) * a) * sqrt(2*M_PI) / (static_cast<double>(2*response.size()*iterations));
}

}