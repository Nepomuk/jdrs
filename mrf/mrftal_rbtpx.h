/*============================================================*/
/* mrftal_rbtpx.h                                             */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* including ToPiX protocol                                   */
/*                                               M.C. Mertens */
/*============================================================*/

#ifndef __MRFTAL_RBTPX_H__
#define __MRFTAL_RBTPX_H__

#include "mrftal_rbudp.h"
#include "mrfdata/mrfdata_tpxflags.h"
#include "mrfdata/mrfdata_ltc2620.h"

//#include "mrfgal_sis1100.h"
//#include "mrfbase.h"

//! Topix Firmware Register Addresses
namespace tpx_address {
	// Register Adresses
	static const mrf::addresstype flags = 0x480;
	static const mrf::addresstype input = 0x484;
	static const mrf::addresstype inputcount = 0x488;
	static const mrf::addresstype datatest = 0x48c;
	static const mrf::addresstype triggercount = 0x490;
	static const mrf::addresstype ltcconfig = 0x494;
	static const mrf::addresstype bussel1data = 0x498;
	static const mrf::addresstype bussel0data = 0x49c;
	static const mrf::addresstype aprechrgdata = 0x4a0;
	static const mrf::addresstype dprechrgdata = 0x4a4;
	static const mrf::addresstype alatchdata = 0x4a8;
	static const mrf::addresstype dlatchdata = 0x4ac;
	static const mrf::addresstype readcmddata = 0x4b0;
	static const mrf::addresstype readledata = 0x4b4;
	static const mrf::addresstype readtedata = 0x4b8;
	static const mrf::addresstype triggerlodata = 0x4bc;
	static const mrf::addresstype triggerhidata = 0x4c0;
	static const mrf::addresstype masterbitcount = 0x4c4;
}

//! Topix Control Register Values
namespace tpxctrl_value {
	//0..7: System --> mrftal_rbbase.h
	//static const mrf::registertype daqreset = 3;			//CM_MODE(0+1)
	//static const mrf::registertype pandareset = 4;		//CM_MODE(2)
	//static const mrf::registertype led0 = 8;			//CM_MODE(3)
	//static const mrf::registertype led1 = 16;			//CM_MODE(4)
	//static const mrf::registertype configdcm = 32;		//CM_MODE(5)

	//8..31: User
	static const mrf::registertype tpxfakedata = 0x100;		//CM_MODE(8)
    static const mrf::registertype lcdstart = 0x800;        //CM_MODE(11)
	static const mrf::registertype startshifter = 0x1000;		//CM_MODE(12)
	static const mrf::registertype startmaster = 0x2000;		//CM_MODE(13)
	static const mrf::registertype ltcconfig = 0x4000;		//CM_MODE(14)
	static const mrf::registertype triggerread = 0x8000;		//CM_MODE(15)
	static const mrf::registertype triggerwithlo = 0x10000;	//CM_MODE(16)
}

//! Readout Board Interrupts
/*
namespace rb_irq {
	static const u_int32_t button = 0x2;
	//static const u_int32_t strobeidle = 0x200;
	//static const u_int32_t inputidle = 0x800;
}*/

//! Error flags set by functions accessing the Panda Readout Board
/*namespace rb_error {
	static const u_int32_t success = 0;
	static const u_int32_t read_failed = 1;
	static const u_int32_t initreadout_failed = 4;
	static const u_int32_t doreadout_failed = 8;
	static const u_int32_t more_data_available = 16;
	static const u_int32_t device_not_open = 32;
}*/

//! Transport access layer (Panda Readout Board specific).
/*!
Provides an interface to the Panda Readout Board.
*/
class TMrfTal_RBTopix : virtual public TMrfTal_RBUDP
{
	public:
		TMrfTal_RBTopix();
		virtual ~TMrfTal_RBTopix();

		// From TMrfGal
		//! Opens the device file and enables essential IRQs
		//virtual void openDevice(const char* const devicefile);

		// From TMrfTal
		//! Sends the stream stored in data to the connected frontend.
		virtual void writeRemoteData(const TMrfData& data) const;
		
		//! Retrieves data from the readout board's buffer.
		virtual void readRemoteData(TMrfData& data) const;

		virtual void triggerRead(const u_int32_t& triggercount, const bool& withlo) const;

		virtual void configTopixSlowReg(const TMrfData_TpxFlags& data) const;

                virtual void writeLTCData(const TMrfData_LTC& data) const;
		
		virtual void startShifter(const u_int32_t& count) const;
		
		virtual void startMaster() const;

		//! Arms the data output buffer
		/*!
		Prepares the output buffer to accept a certain amount of data. Needs to be called before data can be read out to the PC.
		\param datacount: Number of dwords to be stored in the output buffer for later retrieval.

		Error codes are set by the underlying write() functions.
		*/
		//void prepareOutputBuffer(const mrf::registertype& datacount) const;

		//! Arms the event output buffer.
		/*!
		Prepares the output buffer to accept event data with EOE markers.
		*/
		//void prepareEventOutputBuffer(const mrf::registertype& eventcount = 1) const;

	protected:

	private:
		//u_int32_t* buf;
		//bool databuffered;
};

#endif /* __MRFTAL_RBTPX_H__ */
