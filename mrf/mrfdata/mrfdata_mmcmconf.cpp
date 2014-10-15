/*============================================================*/
/* mrfdata_mmcmconf.h                                         */
/* MVD Readout Framework Data Storage                         */
/* Specific to Virtex6 MMCM_ADV Data Format                   */
/*                                               S.Esch		  */
/*============================================================*/

#include "mrfdata_mmcmconf.h"
#include <iostream>
#include <math.h>
#include <sys/types.h> // wgn u_int64_t

TMrfData_MMCMConf::TMrfData_MMCMConf(const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse)
: TMrfDataAdv1D(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),TMrfDataAdv1DAddress(blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
  {
	initMaps();

}

void TMrfData_MMCMConf::initMaps()
{
	_localdata.clear();
	_localdata["register0"] = TConfItem(0,mmcm_address::mmcmconfigreg0,8);
	_localdata["register1"] = TConfItem(0,mmcm_address::mmcmconfigreg1,8);
	_localdata["register2"] = TConfItem(0,mmcm_address::mmcmconfigreg2,8);
	_localdata["register3"] = TConfItem(0,mmcm_address::mmcmconfigreg3,8);
	_localdata["register4"] = TConfItem(0,mmcm_address::mmcmconfigreg4,8);
	_localdata["register5"] = TConfItem(0,mmcm_address::mmcmconfigreg5,8);
	_localdata["register6"] = TConfItem(0,mmcm_address::mmcmconfigreg6,8);
	_localdata["register7"] = TConfItem(0,mmcm_address::mmcmconfigreg7,8);
	_localdata["register8"] = TConfItem(0,mmcm_address::mmcmconfigreg8,8);
	_localdata["register9"] = TConfItem(0,mmcm_address::mmcmconfigreg9,8);
	_localdata["register10"] = TConfItem(0,mmcm_address::mmcmconfigreg10,8);
	_localdata["register11"] = TConfItem(0,mmcm_address::mmcmconfigreg11,8);
	_localdata["register12"] = TConfItem(0,mmcm_address::mmcmconfigreg12,8);
	_localdata["register13"] = TConfItem(0,mmcm_address::mmcmconfigreg13,8);
	_localdata["register14"] = TConfItem(0,mmcm_address::mmcmconfigreg14,8);
	_localdata["register15"] = TConfItem(0,mmcm_address::mmcmconfigreg15,8);
	_localdata["register16"] = TConfItem(0,mmcm_address::mmcmconfigreg16,8);
	_localdata["register17"] = TConfItem(0,mmcm_address::mmcmconfigreg17,8);
	_localdata["register18"] = TConfItem(0,mmcm_address::mmcmconfigreg18,8);
	_localdata["register19"] = TConfItem(0,mmcm_address::mmcmconfigreg19,8);
	_localdata["register20"] = TConfItem(0,mmcm_address::mmcmconfigreg20,8);
	_localdata["register21"] = TConfItem(0,mmcm_address::mmcmconfigreg21,8);
	_localdata["register22"] = TConfItem(0,mmcm_address::mmcmconfigreg22,8);
}

TMrfData_MMCMConf::~TMrfData_MMCMConf() {

}

const double& TMrfData_MMCMConf::getFrequency() const
{
	return _frequency;
}

const double& TMrfData_MMCMConf::setFrequency(const double &frequency)
{
	if(frequency==0){
		_frequency = 0;
	}
	else{
		_frequency = calc_fout(frequency);
		calculate_mmcm_register();
	}
	return _frequency;
}

u_int64_t TMrfData_MMCMConf::mmcm_count_calc(u_int divide, int32_t phase, u_int32_t duty_cycle)
{
	u_int		div_calc,phase_calc;

	div_calc=mmcm_divider(divide, duty_cycle);
	phase_calc=mmcm_phase(divide, phase);
	return (((((((((((phase_calc>>9)&0x3)<<2)|((div_calc>>12)&0x3))<<6)|(phase_calc&0x3F))<<3)
			|((phase_calc>>6)&0x7))<<1)|0)<<12)|(div_calc&0xFFF);
}

u_int16_t TMrfData_MMCMConf::mmcm_divider(u_int divide, u_int duty_cycle)
{

	u_int		duty_cycle_fix;
	u_int		high_time,low_time;
	u_int		w_edge,no_count;
	u_int		temp;

	if (duty_cycle >= 100000) {
		std::cout << "ERROR: mmcm_divider, duty_cycle" << std::endl;
		return 0;
	}

	duty_cycle_fix=(duty_cycle<<FRAC_PRECISION)/100000;
	if (divide == 1) { high_time=1; w_edge=0; low_time=1; no_count=1; }
	else {
		temp=round_frac(duty_cycle_fix*divide, 1);
		high_time=(temp>>FRAC_PRECISION)&0x7F;
		w_edge=temp>>(FRAC_PRECISION-1)&1;

		if (high_time == 0) { high_time=1; w_edge=0; }

		if (high_time == divide) { high_time=divide-1; w_edge=1; }

		low_time=divide-high_time;
		no_count=0;
	}

	return (((((w_edge<<1)|no_count)<<6)|(high_time&0x3F))<<6)|(low_time&0x3F);
}

int TMrfData_MMCMConf::round_frac(int decimal, int pricision)
{

	if (pricision >= FRAC_PRECISION) {
		std::cout << "error round_frac" << std::endl;
		return -0;
	}
	if (decimal&(1<<(FRAC_PRECISION-pricision-1))) return decimal+(1<<(FRAC_PRECISION-pricision-1));
	return decimal;
}

int TMrfData_MMCMConf::mmcm_phase(int divide, int phase)
{
	u_int		phase_in_cycles,phase_fixed;
	u_int		mx,delay_time,phase_mux;
	u_int		temp;

	if ((phase < -360000) || (phase > 360000)) {
        //printf("ERROR: mmcm_phase\n");
		return 0;
	}

	if (phase < 0) phase_fixed=((phase+360000)<<FRAC_PRECISION)/1000;
	else phase_fixed=(phase<<FRAC_PRECISION)/1000;

	phase_in_cycles=(phase_fixed*divide)/360;
	temp=round_frac(phase_in_cycles, 3);
	mx=0;
	phase_mux=(temp>>(FRAC_PRECISION-3))&0x7;
	delay_time=(temp>>(FRAC_PRECISION))&0x3F;

	return (((mx<<3)|phase_mux)<<6)|delay_time;
}


u_int64_t TMrfData_MMCMConf::mmcm_lock_lookup(u_int divide)
{

	return mmcm_lookup::lookup[64-divide];
}

u_int16_t TMrfData_MMCMConf::mmcm_filter_lookup(u_int divide, bool bandwidth)
{
	//if (!strcmp(bandwidth, "LOW")) return mmcm_lookup::lookup_low[64-divide];
	if(bandwidth)
	{
		return mmcm_lookup::lookup_low[64-divide];
	}
	else
	{
		return mmcm_lookup::lookup_high[64-divide];
	}
}

double TMrfData_MMCMConf::calc_fout(double fout)
{
		u_int	m,d,o;
		u_int	mx,dx,ox;
		double	diff;
		double	vco,fo;
		double fout_is =0;

		mx=0; dx=0; ox=0; fout_is=0.0; diff=1.0;
		for (m=5; m <= 64; m++) {
			for (d=1; d <= 80; d++) {
				vco=(CLKIN*m)/d;
				if (vco > VCO_MAX) continue;
				if (vco < VCO_MIN) break;
				for (o=1; o <= 128; o++) {
					fo=vco/o;
					if (fabs(fo-fout) < diff) {
						diff=fabs(fo-fout); fout_is=fo;
						mx=m; dx=d; ox=o;
					}
				}
			}
		}

		if (mx == 0) {
			std::cout << "no matching "<< std::endl;
			return 0;
		}
		clkfbout_mult	=mx;
		divclk_divide	=dx;
		clkout0_divide	=ox;
		clkout1_divide	=ox;
		clkout2_divide	=ox;
		clkout3_divide	=ox;
		clkout4_divide	=ox;
		clkout5_divide	=ox;
		clkout6_divide	=ox;

		std::cout << "required = "<< fout << "MHz" <<std::endl;
		std::cout << "M " << mx << " D " << dx << " 0= " << ox << " Fout = " << fout_is << "MHz (" << 1000./fout_is << "ns)" << std::endl;
		return fout_is;
}


void TMrfData_MMCMConf::calculate_mmcm_register()
{
	CLKFBOUT       = mmcm_count_calc(clkfbout_mult, CLKFBOUT_PHASE, 50000);
	DIGITAL_FILT   = mmcm_filter_lookup(clkfbout_mult, BANDWIDTH_LOW);
	LOCK           = mmcm_lock_lookup(clkfbout_mult);
	DIVCLK         = mmcm_count_calc(divclk_divide, 0, 50000);
	CLKOUT0        = mmcm_count_calc(clkout0_divide, CLKOUT0_PHASE, CLKOUT0_DUTY);
	CLKOUT1        = mmcm_count_calc(clkout1_divide, CLKOUT1_PHASE, CLKOUT1_DUTY);
	CLKOUT2        = mmcm_count_calc(clkout2_divide, CLKOUT2_PHASE, CLKOUT2_DUTY);
	CLKOUT3        = mmcm_count_calc(clkout3_divide, CLKOUT3_PHASE, CLKOUT3_DUTY);
	CLKOUT4        = mmcm_count_calc(clkout4_divide, CLKOUT4_PHASE, CLKOUT4_DUTY);
	CLKOUT5        = mmcm_count_calc(clkout5_divide, CLKOUT5_PHASE, CLKOUT5_DUTY);
	CLKOUT6        = mmcm_count_calc(clkout6_divide, CLKOUT6_PHASE, CLKOUT6_DUTY);

	setLocalItemValue("register0", (u_int32_t)(_UL(0x280000)<<16)|0xFFFF);

      // Store CLKOUT0 divide and phase
	setLocalItemValue("register1",(_UL(0x081000)<<16)|(CLKOUT0&0xFFFF));

	setLocalItemValue("register2",(_UL(0x09FC00)<<16)|(CLKOUT0>>16));

      // Store CLKOUT1 divide and phase
	setLocalItemValue("register3",(_UL(0x0A1000)<<16)|(CLKOUT1&0xFFFF));
	setLocalItemValue("register4",(_UL(0x0BFC00)<<16)|(CLKOUT1>>16));

      // Store CLKOUT2 divide and phase
	setLocalItemValue("register5",(_UL(0x0C1000)<<16)|(CLKOUT2&0xFFFF));
	setLocalItemValue("register6",(_UL(0x0DFC00)<<16)|(CLKOUT2>>16));

      // Store CLKOUT3 divide and phase
	setLocalItemValue("register7",(_UL(0x0E1000)<<16)|(CLKOUT3&0xFFFF));
	setLocalItemValue("register8",(_UL(0x0FFC00)<<16)|(CLKOUT3>>16));

      // Store CLKOUT4 divide and phase
	setLocalItemValue("register9",(_UL(0x101000)<<16)|(CLKOUT4&0xFFFF));
	setLocalItemValue("register10",(_UL(0x11FC00)<<16)|(CLKOUT4>>16));

      // Store CLKOUT5 divide and phase
	setLocalItemValue("register11",(_UL(0x061000)<<16)|(CLKOUT5&0xFFFF));
	setLocalItemValue("register12",(_UL(0x07FC00)<<16)|(CLKOUT5>>16));

      // Store CLKOUT6 divide and phase
	setLocalItemValue("register13",(_UL(0x121000)<<16)|(CLKOUT6&0xFFFF));
	setLocalItemValue("register14",(_UL(0x13FC00)<<16)|(CLKOUT6>>16));

      // Store the input divider
	setLocalItemValue("register15",(((((_UL(0x16C000)<<2)|0)<<2)|((DIVCLK>>22)&0x3))<<12)|(DIVCLK&0xFFF));

      // Store the feedback divide and phase
	setLocalItemValue("register16",(_UL(0x141000)<<16)|(CLKFBOUT&0xFFFF));
	setLocalItemValue("register17",(_UL(0x15FC00)<<16)|(CLKFBOUT>>16));

      // Store the lock settings
	setLocalItemValue("register18",(((_UL(0x18FC00)<<6)|0)<<10)|((LOCK>>20)&0x3FF));
	setLocalItemValue("register19",(((((_UL(0x198000)<<1)|0)<<5)|((LOCK>>30)&0x1F))<<10)|(LOCK&0x3FF));
	setLocalItemValue("register20",(((((_UL(0x1A8000)<<1)|0)<<5)|((LOCK>>35)&0x1F))<<10)|((LOCK>>10)&0x3FF));

      // Store the filter settings
	setLocalItemValue("register21",(((((((((((_UL(0x4E66FF)<<1)|((DIGITAL_FILT>>9)&1))<<2)|0)<<2)
					|((DIGITAL_FILT>>7)&3))<<2)|0)<<1)|((DIGITAL_FILT>>6)&1))<<8)|0);
	setLocalItemValue("register22",(((((((((((((((_UL(0x4F666F)<<1)|((DIGITAL_FILT>>5)&1))<<2)|0)<<2)|((DIGITAL_FILT>>3)&3))<<2)
					|0)<<2)|((DIGITAL_FILT>>1)&3))<<2)|0)<<1)|(DIGITAL_FILT&1))<<4)|0);
}


