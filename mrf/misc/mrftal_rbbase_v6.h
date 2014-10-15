/*============================================================*/
/* mrftal_rbbase_v6.h                                         */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* Basic Functions for readoutboard with Virtex 6             */
/*                                               S. Esch	  */
/*============================================================*/

#ifndef MRFTAL_RBBASE_V6_H_
#define MRFTAL_RBBASE_V6_H_

#include "mrftal_rbbase.h"
#include "mrfdata/mrfdata_mmcmconf.h"

class TMrfTal_RBBase_V6: public virtual TMrfTal_RBBase {
public:
	TMrfTal_RBBase_V6();
	virtual ~TMrfTal_RBBase_V6();

	//! Configures the Virtex6 MMCM_ADV frequency generator

	/*!
	This is not the usual configuration like in the documentation.
	A special program from Willi Erven calculates all registers.
	With this workaround it is possible to change the clock online with a stepsize ~ 50 kHz

	\params freq frequency in MHz
	*/

	virtual const double& configMMCM(const double& freq);

private:
	TMrfData_MMCMConf _mmcmconf;

};

#endif /* MRFTAL_RBBASE_V6_H_ */
