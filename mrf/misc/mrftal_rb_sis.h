/*============================================================*/
/* mrftal_rb.h                                                */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via SIS1100 Gigabit Link                         */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFTAL_RB_H__
#define __MRFTAL_RB_H__

#include "mrftal_rbbase.h"
#include "mrfbase.h"


//! Important registers of the Panda Readout Board
namespace rb_address {
	// Register Addresses
	static const mrf::addresstype strblen = 1048;		//0x418
	static const mrf::addresstype lvl1delay = 1052;		//0x41C
	static const mrf::addresstype timer = 1056;		//0x420
	static const mrf::addresstype inputreg = 1060;		//0x424
	static const mrf::addresstype bitcountreg = 1064;	//0x428
	static const mrf::addresstype lvl1length = 1068;	//0x42C
	static const mrf::addresstype synclength = 1072;	//0x430
	static const mrf::addresstype bitcountoreg = 1076;	//0x434
	// Register Addresses in RBBase
	//static const mrf::addresstype ident = 1024;		//0x400
	//static const mrf::addresstype doorbell = 1028;		//0x404
	//static const mrf::addresstype control = 1032;		//0x408
	//static const mrf::addresstype busy = 1040;		//0x410
	//static const mrf::addresstype dcmconfigreg = 1092;	//0x444
}

//! Important register values of the Panda Readout Board
namespace rb_value {	
	static const mrf::registertype startinput = 32;		//Bit 5
	static const mrf::registertype startsync = 64;		//Bit 6
	static const mrf::registertype strblvl1 = 128;		//Bit 7
	static const mrf::registertype eoedetect = 256;		//Bit 8
	static const mrf::registertype outputenable = 512;	//Bit 9
	static const mrf::registertype xckrenable = 1024;	//Bit A
	// These are defined in RBBase
	//static const mrf::registertype daqreset = 3;		//Bits 0+1
	//static const mrf::registertype pandareset = 4;		//Bit 2
	//static const mrf::registertype led0 = 8;		//Bit 3
	//static const mrf::registertype led1 = 16;		//Bit 4
	//static const mrf::registertype configdcm = 2048;	//Bit B
}

//! Readout Board Interrupts
namespace rb_irq {
	static const u_int32_t strobeidle = 0x200;
	static const u_int32_t inputidle = 0x800;
	// This is now defined in RBBase
	//static const u_int32_t button = 0x2;
}

//! Transport access layer (Panda Readout Board specific).
/*!
Provides an interface to the Panda Readout Board.
*/
class TMrfTal_RB : virtual public TMrfTal_RBBase
{
	public:
		TMrfTal_RB();
		virtual ~TMrfTal_RB();

		// From TMrfGal
		//! Opens the device file and enables essential IRQs
		virtual void openDevice(const char* const devicefile);

		// From TMrfTal
		//! Sends the stream stored in data to the connected frontend.
		virtual void writeRemoteData(const TMrfData& data) const;
		//! Retrieves data from the readout board's buffer.
		virtual void readRemoteData(TMrfData& data) const;

		//! Retrieves identification information stored in the FPGA.
		/*!
		\return Identification register of FPGA.
		*/
		mrf::registertype getFpgaID() const;


		//! Executes a Panda Reset on the Readout Board.
		void pandaReset() const;

		//! Sends a strobe signal of given length to the connected FE.
		/*!
		\param strobelength Takes a value between 1 and 512 which gives the length of the strobe.
		\param levelonedelay Delay until level 1 trigger is fired after strobe (1 to 512).
		\param levelonelength Length of level 1 trigger signal (1 to 16, continuous???).
		*/
		void sendStrobe(const u_int32_t& strobelength, const u_int32_t& levelonedelay, const u_int32_t& levelonelength) const;

		//! Sends a level 1 trigger of given length to the connected FE.
		/*!
		\param levelonelength Length of level 1 trigger signal (1 to 16, continuous???).
		*/
		void sendLevelOneTrigger(const u_int32_t& levelonelength) const;

		//! Sends sync signal (reset) of given length to the connected FE.
		/*!
		\param synclength Length of sync signal.
			<4 clock cycles: Sync,
			<8 clock cycles: Soft Reset,
			>8 clock cycles: Hard Reset
		*/
		void sendSync(const u_int32_t& synclength) const;

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
		void setLedState(const u_int32_t& led_id, const bool& state) const;

		//! Retrieves the state of the LEDs on the SIS1100 card.
		/*!
		The LEDs are positioned as follows:
		0AB
		1CD
		TODO: Add description of A, B, C, D
		\param led_id Takes the value 0 or 1 to retrieve either LED's state.
		\return \b True if the tested LED is on, \b False if the tested LED is off.
		*/
		bool getLedState(const u_int32_t& led_id) const;

		//! Arms the data output buffer
		/*!
		Prepares the output buffer to accept a certain amount of data. Needs to be called before data can be read out to the PC.
		\param datacount: Number of dwords to be stored in the output buffer for later retrieval.

		Error codes are set by the underlying write() functions.
		*/
		void prepareOutputBuffer(const mrf::registertype& datacount) const;

		//! Arms the event output buffer.
		/*!
		Prepares the output buffer to accept event data with EOE markers.
		*/
		void prepareEventOutputBuffer(const mrf::registertype& eventcount = 1) const;

		//! Retrieves data from output buffer.
		/*!
		The data which is currently stored in the readout board output buffer is transferred to a registerdata structure.
		\param data: TMrfData structure which will hold the retrieved data.
		\param wordcount: Number of dwords to retrieve.
		\param appendat: Start position at which data is appended.
		\param truncate: When true (default), the data memory will be truncated to the amount of data. When false, the buffer remains at least as large as the data (useful for anticipatory memory allocation).
		*/
		int readOutputBuffer(TMrfData& data, const u_int32_t& wordcount, const u_int32_t& appendat = 0, const bool& truncate = true) const;

		//! Determins if the board is busy.
		/*!
		\return \b True if the board is currently executing an operation, \b False if the board is idle.
		*/
		bool isBusy(const mrf::registertype& mask) const;

		//!Returns the internally stored error code for readout board operations.
		/*!
		\return An integer with errorflags of all errors occured after last successful command. Zero returned means success.
		*/
		const u_int32_t& getLastRBError() const;

		//! Determines if the last executed readout board function completed successfully.
		/*!
		\return \b True if the last action completed successfullly, \b False if any error occured.
		*/
		bool lastRBActionSuccessful() const;

		//! Configures the Virtex4 DCM_ADV frequency generator.
		/*!
		Synthesizes a m/d * 100MHz frequency. Note that for Virtex4, the resulting frequency must be between 32MHz and 210MHz. This corresponds to m/d between 1/3 and 21/10.
		\param m: Clock multiply by. Ranges from 2 to 32.
		\param d: Clock divide by. Ranges from 2 to 32.
		*/
		void configDCM(const int& m, const int& d) const;

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


#endif // __MRFTAL_RB_H__

