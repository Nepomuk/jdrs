/*============================================================*/
/* mrftal_rbudp.h                                             */
/* MVD Readout Framework Transport Access Layer               */
/* Transport Access Layer for Panda Readout Development Board */
/* connected via UDP Ethernet Link                            */
/* Basic Functions                                            */
/*                                                 A. Goerres */
/*============================================================*/


#ifndef __MRFTAL_RBUDP_H__
#define __MRFTAL_RBUDP_H__

#include "mrftal.h"
#include "mrfgal_udp.h"
#include "mrfbase.h"
#include "mrfdata/mrfdataregaccess.h"
#include "mrfdata/mrfdataadv1daddress.h"
#include "mrfdata/mrfdata_mmcmconf.h"




//! Important registers of the Panda Readout Board
namespace rb_address {
    // Register Adresses
    static const mrf::addresstype ident = 1024;		//0x400
    static const mrf::addresstype doorbell = 1028;	//0x404
    static const mrf::addresstype control = 8;		//0x408   // vorher 1032 - 0x408
    static const mrf::addresstype busy = 1040;		//0x410
    static const mrf::addresstype dcmconfigreg = 0x414;	//0x414
    static const mrf::addresstype ledlinereg = 0x41c;	//0x41c

    // from mrftal_rb.h
    static const mrf::addresstype strblen = 1048;	//0x418
    static const mrf::addresstype lvl1delay = 1052;	//0x41C
    static const mrf::addresstype timer = 1056;		//0x420
    static const mrf::addresstype inputreg = 1060;	//0x424
    static const mrf::addresstype bitcountreg = 1064;	//0x428
    static const mrf::addresstype lvl1length = 1068;	//0x42C
    static const mrf::addresstype synclength = 1072;	//0x430
    static const mrf::addresstype bitcountoreg = 1076;	//0x434

    // LCD stuff
    static const mrf::addresstype lcd_mode = 0x4cc;     //0x4cc
    static const mrf::addresstype lcd_fifo = 0x4d0;     //0x4d0
}



//! Important register values of the Panda Readout Board
namespace rb_value {
	static const mrf::registertype daqreset = 3;		//Bits 0+1
	static const mrf::registertype pandareset = 4;		//Bit 2
	static const mrf::registertype led0 = 8;		//Bit 3
	static const mrf::registertype led1 = 16;		//Bit 4
	static const mrf::registertype configdcm = 32;		//Bit 5
	static const mrf::registertype configmmcm = 32;		//Bit 5

        // from mrftal_rb.h
        static const mrf::registertype startinput = 32;		//Bit 5
        static const mrf::registertype startsync = 64;		//Bit 6
        static const mrf::registertype strblvl1 = 128;		//Bit 7
        static const mrf::registertype eoedetect = 256;		//Bit 8
        static const mrf::registertype outputenable = 512;	//Bit 9
        static const mrf::registertype xckrenable = 1024;	//Bit A
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
class TMrfTal_RBUDP : virtual public TMrfTal, virtual public TMrfGal_Udp
{
    public:
        TMrfTal_RBUDP();
        virtual ~TMrfTal_RBUDP();

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

        // From TMrfTal
        //! Sends the stream stored in data to the connected frontend.
        virtual void writeRemoteData(const TMrfData& data) const;
        //! Retrieves data from the readout board's buffer.
        virtual void readRemoteData(TMrfData& data) const;


        //! Configures the Virtex6 MMCM_ADV frequency generator

        /*!
        This is not the usual configuration like in the documentation.
        A special program from Willi Erven calculates all registers.
        With this workaround it is possible to change the clock online with a stepsize ~ 50 kHz

        \params freq frequency in MHz
        */
        virtual const double& configMMCM(const double& freq);

        protected:
        //! Initializes DMA readout.
        int initReadout() const;

        //! Executes DMA readout.
        //const int doReadout(TMrfData& data, const u_int32_t& wordcount, u_int8_t* const startaddress);
        int doReadout(const u_int32_t& wordcount, u_int8_t* const startaddress) const;



        //! Error code for readout board commands
        /*!

        */
        mutable u_int32_t rb_errcode;


    private:
        TMrfData_MMCMConf _mmcmconf;
        //u_int32_t* buf;
        //bool databuffered;


};


#endif // __MRFTAL_RBUDP_H__

