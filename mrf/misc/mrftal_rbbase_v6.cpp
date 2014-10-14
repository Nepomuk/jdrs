/*============================================================*/
/* mrftal_rbbase_v6.h                                         */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* Basic Functions for readoutboard with Virtex 6             */
/*                                               S. Esch      */
/*============================================================*/

#include "mrftal_rbbase_v6.h"

TMrfTal_RBBase_V6::TMrfTal_RBBase_V6()
: TMrfTal_RBBase()
{

}

TMrfTal_RBBase_V6::~TMrfTal_RBBase_V6()
{

}

const double& TMrfTal_RBBase_V6::configMMCM(const double& freq)
{

	if(deviceIsOpen()){
		_mmcmconf.setFrequency(freq);
		writeRegisterDataAddress(_mmcmconf);
		write(mmcm_address::mmcmconfigregister,00000000000000000000000000000010);
		return _mmcmconf.getFrequency();
	} else {
		errcode |= mrf_error::device_not_open;
		_mmcmconf.setFrequency(0);
		return _mmcmconf.getFrequency();
	}
}
