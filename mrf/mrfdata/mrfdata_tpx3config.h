/*============================================================*/
/* mrfdata_tpxpixel.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Provides Access to ToPiX Pixel Configuration               */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFDATA_TPX3CONFIG_H__
#define __MRFDATA_TPX3CONFIG_H__

#include "mrfdataadv1d.h"

namespace topix3_command {
        static const u_int32_t storedataccr0 = 0;
        static const u_int32_t readdataccr0 = 1;
        static const u_int32_t storedataccr1 = 2;
        static const u_int32_t readdataccr1 = 3;
        static const u_int32_t storedataccr2 = 4;
        static const u_int32_t readdataccr2 = 5;
        static const u_int32_t writepixelconfigeven = 8;
        static const u_int32_t writepixelconfigodd = 9;
        static const u_int32_t readpixelconfigeven = 10;
        static const u_int32_t readpixelconfigodd = 11;
        static const u_int32_t movetonextpixeleven = 12;
        static const u_int32_t movetonextpixelodd = 13;
        static const u_int32_t normaloperation = 14;
        static const u_int32_t configmodeoperation = 15;
        static const u_int32_t nooperation = 31;
}

namespace topix3_ccrnumber {
        static const std::string ccr0 = "CommandCCR0";
        static const std::string ccr1 = "CommandCCR1";
        static const std::string ccr2 = "CommandCCR2";
}

class TMrfData_Tpx3Config : virtual public TMrfDataAdv1D
{
	public:
                TMrfData_Tpx3Config(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = false, const bool& defaultstreamreverse = false);

		//From TMrfDataAdvBase
		virtual void initMaps();
                virtual void assemble();
                //virtual void disassemble();

                virtual void setCommand(std::string ccr, u_int32_t command);

	protected:

	private:

};

#endif /* __MRFDATA_TPX3CONFIG_H__ */
