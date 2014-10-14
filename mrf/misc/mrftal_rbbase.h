/*============================================================*/
/* mrftal_rbbase.h                                            */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/* Basic Functions                                            */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFTAL_RBBASE_H__
#define __MRFTAL_RBBASE_H__

#include "mrftal.h"
#include "mrfgal_udp.h"
#include "mrfbase.h"
#include "mrfdata/mrfdataregaccess.h"
#include "mrfdata/mrfdataadv1daddress.h"



//! Important registers of the Panda Readout Board
namespace rb_address {
	// Register Adresses
	static const mrf::addresstype ident = 1024;		//0x400
	static const mrf::addresstype doorbell = 1028;		//0x404
	static const mrf::addresstype control = 8;		//0x408   // vorher 1032 - 0x408
	static const mrf::addresstype busy = 1040;		//0x410
    static const mrf::addresstype dcmconfigreg = 0x414;	//0x414
    static const mrf::addresstype ledlinereg = 0x41c;	//0x41c

    // LCD stuff
    static const mrf::addresstype lcd_mode = 0x4cc;    //0x4cc
    static const mrf::addresstype lcd_fifo = 0x4d0;    //0x4d0
}



//! Important register values of the Panda Readout Board
namespace rb_value {
	static const mrf::registertype daqreset = 3;		//Bits 0+1
	static const mrf::registertype pandareset = 4;		//Bit 2
	static const mrf::registertype led0 = 8;		//Bit 3
	static const mrf::registertype led1 = 16;		//Bit 4
	static const mrf::registertype configdcm = 32;		//Bit 5
	static const mrf::registertype configmmcm = 32;		//Bit 5
}

//! Readout Board Interrupts
namespace rb_irq {
	static const u_int32_t button = 0x2;
}

//! Positions of the LED enable/disable flags of the Panda Readout Board
namespace sis_led {
	//enum led_id {
	//	led0 = 8,
	//	led1 = 16,
	//};
	static const mrf::registertype led[2] = {rb_value::led0, rb_value::led1};
}

/*
// Doorbell Register Values
#define SIS_VAL_DWVALID			1			//	Bit 0
#define SIS_VAL_PANDASW1		2			//	Bit 1
#define SIS_VAL_IFIFOFULL		4			//	Bit 2
#define SIS_VAL_EOE				8			//	Bit 3
*/

//! Error flags set by functions accessing the Panda Readout Board
namespace rb_error {
	static const u_int32_t success = 0;
	static const u_int32_t read_failed = 1;
	static const u_int32_t initreadout_failed = 4;
	static const u_int32_t doreadout_failed = 8;
	static const u_int32_t more_data_available = 16;
	static const u_int32_t device_not_open = 32;
}

//! Magic numbers used by SIS 1100 interface
namespace sis_magic {
	static const u_int32_t success = 0;
	static const u_int32_t nomoredata = 0x209;
}

//! Transport access layer (Panda Readout Board specific).
/*!
Provides an interface to the Panda Readout Board.
*/
class TMrfTal_RBBase : virtual public TMrfTal, virtual public TMrfGal_Udp //TMrfGal_Sis1100
{
	public:
		TMrfTal_RBBase();
		virtual ~TMrfTal_RBBase();

		//! Retrieves identification information stored in the FPGA.
		/*!
		\return Identification register of FPGA.
		*/
		virtual mrf::registertype getFpgaID() const;

		//! Executes a Panda Reset on the Readout Board.
		virtual void pandaReset() const;

		//! Switches the LEDs on the SIS1100 card on and off.
		/*!
		The two user controllable LEDs of remote SIS1100 card located on the readout board can be switched on and off using this command. The LEDs are positioned as follows:
		0AB
		1CD
		TODO: Add description of A, B, C, D
		\param led_id Takes the value 0 or 1 to set either LED's state.
		\param state true switches the LED on, false switches the LED off

		<b>Error codes:</b>
		*/
		virtual void setLedState(const u_int32_t& led_id, const bool& state) const;

		//! Retrieves the state of the LEDs on the SIS1100 card.
		/*!
		The LEDs are positioned as follows:
		0AB
		1CD
		TODO: Add description of A, B, C, D
		\param led_id Takes the value 0 or 1 to retrieve either LED's state.
		\return \b True if the tested LED is on, \b False if the tested LED is off.
		*/
		virtual bool getLedState(const u_int32_t& led_id) const;

		//! Retrieves data from output buffer.
		/*!
		The data which is currently stored in the readout board output buffer is transferred to a registerdata structure.
		\param data: TMrfData structure which will hold the retrieved data.
		\param wordcount: Number of dwords to retrieve.
		\param appendat: Start position at which data is appended.
		\param truncate: When true (default), the data memory will be truncated to the amount of data. When false, the buffer remains at least as large as the data (useful for anticipatory memory allocation).
		*/
		virtual int readOutputBuffer(TMrfData& data, const u_int32_t& wordcount, const u_int32_t& appendat = 0, const bool& truncate = true) const;

		//! Determins if the board is busy.
		/*!
		\return \b True if the board is currently executing an operation, \b False if the board is idle.
		*/
		virtual bool isBusy(const mrf::registertype& mask) const;

		//!Returns the internally stored error code for readout board operations.
		/*!
		\return An integer with errorflags of all errors occured after last successful command. Zero returned means success.
		*/
		virtual const u_int32_t& getLastRBError() const;

		//! Determines if the last executed readout board function completed successfully.
		/*!
		\return \b True if the last action completed successfullly, \b False if any error occured.
		*/
		virtual bool lastRBActionSuccessful() const;

		//! Configures the Virtex4 DCM_ADV frequency generator.
		/*!
		Synthesizes a (m+1)/(d+1) * 100MHz frequency. Note that for Virtex4, the resulting frequency must be between 32MHz and 210MHz. This corresponds to m/d between 1/3 and 21/10.
		\param m: Clock multiply by. Ranges from 1 to 31.
		\param d: Clock divide by. Ranges from 1 to 31.
		*/
		virtual void configDCM(const int& m, const int& d) const;
		

		virtual u_int32_t getLedLine() const;

		virtual void boardCommand(const mrf::registertype& flag, const bool& status = true) const;
		
		//! Configures value registers according to a TMrfDataRegAccess stored configuration
		virtual void writeRegister(const TMrfDataRegAccess& data, const std::string& item) const;

		//! Configures flag registers according to a TMrfData stored configuration.
		/*!
		Implementation note for TMrfData based class: The flags must be aligned to the data LSB.
		This function uses a 32 Bit wide register.
		 */
		virtual void writeRegisterData(const mrf::addresstype& address, const TMrfData& data) const;

		//! Configures registers according to a TMrfData stored address and configuration


		virtual void writeRegisterDataAddress(const TMrfDataAdv1DAddress& data) const;

	protected:
		//! Initializes SIS1100 DMA readout.
		int initReadout() const;

		//! Executes SIS1100 DMA readout.
		//const int doReadout(TMrfData& data, const u_int32_t& wordcount, u_int8_t* const startaddress);
		int doReadout(const u_int32_t& wordcount, u_int8_t* const startaddress) const;



		//! Error code for readout board commands
		/*!

		*/
		mutable u_int32_t rb_errcode;
	private:
		//u_int32_t* buf;
		//bool databuffered;


};


#endif // __MRFTAL_RBBASE_H__

