/*============================================================*/
/* mrfdata_mmcmconf.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Specific to Virtex6 MMCM_ADV Data Format                   */
/*                                               S.Esch		  */
/*============================================================*/

#ifndef MRFDATA_MMCMCONF_H_
#define MRFDATA_MMCMCONF_H_

#include "mrfdataadv1daddress.h"

#define 	CLKIN				   66.0
#define 	VCO_MIN				   600.0
#define 	VCO_MAX				   1200.0
#define 	FOUT_MIN			   4.69
#define 	FOUT_MAX			   600
#define	CLKFBOUT_MULT          5
#define	CLKFBOUT_PHASE         0
#define	BANDWIDTH_LOW          true
#define	DIVCLK_DIVIDE          1
#define	CLKOUT0_DIVIDE         1
#define	CLKOUT0_PHASE          0
#define	CLKOUT0_DUTY           50000
#define	CLKOUT1_DIVIDE         5
#define	CLKOUT1_PHASE          0
#define	CLKOUT1_DUTY           50000
#define	CLKOUT2_DIVIDE         5
#define	CLKOUT2_PHASE          0
#define	CLKOUT2_DUTY           50000
#define	CLKOUT3_DIVIDE         5
#define	CLKOUT3_PHASE          0
#define	CLKOUT3_DUTY           50000
#define	CLKOUT4_DIVIDE         5
#define	CLKOUT4_PHASE          0
#define	CLKOUT4_DUTY           50000
#define	CLKOUT5_DIVIDE         5
#define	CLKOUT5_PHASE          0
#define	CLKOUT5_DUTY           50000
#define	CLKOUT6_DIVIDE         5
#define	CLKOUT6_PHASE          0
#define	CLKOUT6_DUTY   50000
#define _UL(x)	x##ul
#define FRAC_PRECISION	10

namespace mmcm_address {
static const mrf::addresstype mmcmconfigreg0 = 0x80;
static const mrf::addresstype mmcmconfigreg1 = 0x84;
static const mrf::addresstype mmcmconfigreg2 = 0x88;
static const mrf::addresstype mmcmconfigreg3 = 0x8C;
static const mrf::addresstype mmcmconfigreg4 = 0x90;
static const mrf::addresstype mmcmconfigreg5 = 0x94;
static const mrf::addresstype mmcmconfigreg6 = 0x98;
static const mrf::addresstype mmcmconfigreg7 = 0x9C;
static const mrf::addresstype mmcmconfigreg8 = 0xA0;
static const mrf::addresstype mmcmconfigreg9 = 0xA4;
static const mrf::addresstype mmcmconfigreg10 = 0xA8;
static const mrf::addresstype mmcmconfigreg11 = 0xAC;
static const mrf::addresstype mmcmconfigreg12 = 0xB0;
static const mrf::addresstype mmcmconfigreg13 = 0xB4;
static const mrf::addresstype mmcmconfigreg14 = 0xB8;
static const mrf::addresstype mmcmconfigreg15 = 0xBC;
static const mrf::addresstype mmcmconfigreg16 = 0xC0;
static const mrf::addresstype mmcmconfigreg17 = 0xC4;
static const mrf::addresstype mmcmconfigreg18 = 0xC8;
static const mrf::addresstype mmcmconfigreg19 = 0xCC;
static const mrf::addresstype mmcmconfigreg20 = 0xD0;
static const mrf::addresstype mmcmconfigreg21 = 0xD4;
static const mrf::addresstype mmcmconfigreg22 = 0xD8;
static const mrf::addresstype mmcmconfigregister = 0xFC;
}

//! Lookup Table for mmcm register calculation

namespace mmcm_lookup {

const u_int64_t	lookup[64]={
_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),
_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),
_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFCFAFA401),_UL(0xFFD13FA401),_UL(0xFFD13FA401),
_UL(0xFFD13FA401),_UL(0xFFD2CFA401),_UL(0xFFD2CFA401),_UL(0xFFD2CFA401),_UL(0xFFD45FA401),
_UL(0xFFD45FA401),_UL(0xFFD5EFA401),_UL(0xFFD5EFA401),_UL(0xFFD77FA401),_UL(0xFFD90FA401),_UL(0xFFD90FA401),
_UL(0xFFDA9FA401),_UL(0xFFDC2FA401),_UL(0xFFDDBFA401),_UL(0xFFDF4FA401),_UL(0xFFE0DFA401),_UL(0xFFE26FA401),_UL(0xFFE3FFA401),
_UL(0xFFE71FA401),_UL(0xFFE8AFA401),_UL(0xFFEBCFA401),_UL(0xFFEEEFA401),_UL(0xFFF39FA401),_UL(0xFFF84FA401),_UL(0xE73E8FA401),
_UL(0xCE7E8FA401),_UL(0xB5BE8FA401),_UL(0x9CFE8FA401),_UL(0x8C7E8FA401),
_UL(0x73BE8FA401),_UL(0x5AFE8FA401),_UL(0x423E8FA401),_UL(0x31BE8FA401),
_UL(0x31BE8FA401)
};


const u_int32_t lookup_low[64]={
0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,
0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,0x0A3,
0x0A3,0x093,0x093,0x093,0x093,0x093,0x093,0x093,
0x093,0x093,0x093,0x063,0x063,0x063,0x063,
0x063,0x063,0x063,0x063,0x063,0x063,0x063,0x063,
0x063,0x063,0x053,0x053,0x053,
0x053,0x053,0x053,0x053,0x053,
0x053,0x053,0x0B3,0x04B,0x04B,0x04B,
0x073,0x073,0x073,0x073,0x06B,0x05B,
0x07B,0x057,0x05F
};

const u_int64_t lookup_high[64]={
0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,
0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x0E0,0x110,0x110,0x110,0x110,
0x110,0x110,0x188,0x1F0,0x188,0x188,0x188,
0x3A8,0x330,0x330,0x1C8,0x3E8,0x3E8,0x3E8,0x370,0x370,0x308,0x370,
0x3E8,0x3B0,0x3B0,0x3B0,0x3F0,
0x3F0,0x3F0,0x3F0,0x3F0,0x3F0,0x3F0,0x3F0,
0x3B0,0x3E8,0x3E8,0x3E8,0x3D8,0x3C4,0x3C4,0x3F8,0x3E4,0x3F4,0x3FC,
0x17C
};
}


class TMrfData_MMCMConf: public virtual TMrfDataAdv1DAddress {
public:
	TMrfData_MMCMConf(const u_int32_t& blocklength = bitsinablock, const u_int32_t& defaultindex = 0, const u_int32_t& defaultstreamoffset = 0, const u_int32_t& defaultvalueoffset = 0, const bool& defaultreverse = true, const bool& defaultstreamreverse = false);
	virtual ~TMrfData_MMCMConf();

	//? Returns the set frequency for the MMCM
	/*?
	Returns the frequency which was set via setFrequency()
	 */

	const double& getFrequency() const;

	//? Returns the generatable frequency to the desired frequency

	/*?
	 Not every frequency is generatable in the MMCM. setFrequency() returns the generatable frequency to the desired frequency freq
	 \param freq desired frequency.
	 */
	const double& setFrequency(const double &freq);

	//? Calculates the register for configuration of the MMCM for the frequency set in _frequency.

	void calculate_mmcm_register();

	//? Returns the closest generatable frequency for the desired frequency fout
	virtual double calc_fout(double fout);

	//? Helper function for register calculation
	u_int16_t mmcm_divider(u_int, u_int);

	//? Helper function for register calculation
	int round_frac(int, int);

	//? Helper function for register calculation
	int mmcm_phase(int, int);

	//? Access function for lookup list.
	u_int64_t mmcm_lock_lookup(u_int divide);

	//? Access function for lookup list.
	u_int16_t mmcm_filter_lookup(u_int divide, bool bandwidth);

	//? Access function for lookup list.
	u_int64_t mmcm_count_calc(u_int, int32_t, u_int32_t );

protected:

	void initMaps();

private:

	u_int32_t lookup_low[64];

	u_int64_t	lookup[64];

	u_int64_t lookup_high[64];

	//? Holds the set possible frequency
	double _frequency;
	u_int clkfbout_mult;
	u_int divclk_divide;
	u_int clkout0_divide;
	u_int clkout1_divide;
	u_int clkout2_divide;
	u_int clkout3_divide;
	u_int clkout4_divide;
	u_int clkout5_divide;
	u_int clkout6_divide;
	u_int64_t CLKFBOUT, DIGITAL_FILT, LOCK, DIVCLK;
	u_int64_t CLKOUT0, CLKOUT1, CLKOUT2, CLKOUT3, CLKOUT4, CLKOUT5, CLKOUT6;
};

#endif /* MRFDATA_MMCMCONF_H_ */
