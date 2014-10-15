/*============================================================*/
/* mrfgal_udp.h                                               */
/* MVD Readout Framework Generic Access Layer                 */
/* Generic Access Layer for ethernet based devices            */
/* communicating with the UDP protocol definition.            */
/*                                                 A. Goerres */
/*============================================================*/

#ifndef __MRFGAL_UDP_H__
#define __MRFGAL_UDP_H__

#include <stdlib.h>
#include <vector>
#include <map>

#include "mrfgal.h"
#include "mrfbase.h"
#include "mrfdata.h"

// unfortunately we have to include this file
#include "sis1100_var.h"


//! Define how many bytes we are able to send in one UDP package
#define MAX_BLOCKS_PER_UDP_PACKAGE 368
#define MAX_BYTES_PER_UDP_PACKAGE 1472
#define REG_SIZE 4
#define BULK_WORD_SIZE 4


//! Definitions of the type flag
/*!
 The first byte of the UDP package that is send between the control
 software and the ML605 board is used to identify what should be done.
 */
namespace udpDataFlag {
    // The type flag hast to be the first byte that arrives. Since we are
    // storing 32 bit integers, this would mean that this byte would be
    // at the end (0x00 0x00 0x00 0x##) instead of being in front
    // (0x## 0x00 0x00 0x00). Therefore we are shifting everything three
    // bytes to the left.

    // general flags
    static const u_int32_t nothing	= 0x00 << 3*8;  //!< Well, nothing...
    static const u_int32_t ping		= 0x01 << 3*8;  //!< Send a ping signal.
    static const u_int32_t pong		= 0x02 << 3*8;  //!< A pong is the response to a ping signal.
    static const u_int32_t check	= 0x03 << 3*8;  //!< Everything ok.
    static const u_int32_t error	= 0x04 << 3*8;  //!< An error has occured.
    static const u_int32_t timeout	= 0x05 << 3*8;  //!< There was a timeout.

    // flags for register handling
    static const u_int32_t registerRead		= 0x11 << 3*8;  //!< We want to read out a register value.
    static const u_int32_t registerWrite	= 0x12 << 3*8;  //!< Set a register with a new value.
    static const u_int32_t pkgCountRead		= 0x13 << 3*8;  //!< Read out the package counter in the ML605.
    static const u_int32_t pkgCountReset	= 0x14 << 3*8;  //!< Reset the package counter in the ML605.
    static const u_int32_t bulkRead         = 0x21 << 3*8;  //!< Read in bulk mode from the DAQ fifo
}

//! Define a UDP request, handled by doIoctl
/*!
  Consists of some information, the UDP should do...
  */
typedef struct udpRequest {
    u_int32_t dataFlag;
    mrf::addresstype registerAddress;
    u_int32_t wordcount;
    char* memoryAddress;
} udpRequest;


//! Generic Access Layer (ethernet with UDP)
/*!
Provides an interface to the ethernet communication via UDP standard.
All functions which have an error code set the internal error code to 0 after successful execution.
*/
class TMrfGal_Udp : virtual public TMrfGal
{
    public:
        TMrfGal_Udp();
        virtual ~TMrfGal_Udp();

        //! Opens a device.
        /*!
        \param connectionParameter A string containing the connection parameter in the following format: "192.168.0.1:42000|192.168.0.10:42000" - first the IP:port of the PC where the software runs on, then the IP:port for the board to connect to.

        <b>Error codes:</b>
            - \b 0: Everything OK.
            - \b mrf_error::open_failed: The socket could not be created.
            - \b mrf_error::device_not_open: The connection could not be established.
        */
        virtual void openDevice(const char *const connectionParameter);

        //! Closes an open device.
        /*!
        <b>Error codes:</b>
            - \b 0: Everything OK.
            - \b mrf_error::close_failed: The device could not be closed.
        */
        virtual void closeDevice();


        //! Determines if a socket is open.
        /*!
        The communication with UDP runs via a socket, that is created on the host, running the software. This functions checks, if this socket is open and ready for receiving data.
        \return \b True if the socket is known to be open, \b False otherwise.
        */
        virtual bool deviceIsOpen() const;

        //! Determines if a device is online and connected.
        /*!
        Sends a ping to the ML605 board and checks for a proper pong response.
        \return \b True if the device is known to be online, \b False otherwise.
        */
        virtual bool deviceIsOnline() const;


        //! Executes an ioctl on an open device.
        /*!
        The device must be open.
        This function is overloaded for convenience.
        \param request request to pass to the ioctl command

        <b>Error codes:</b>
            - \b 0: Everything OK.
            - \b mrf_error::device_not_open: The device is not open.
            - \b mrf_error::ioctl_failed: Ioctl failed.
        */
        virtual int doIoctl(const int& request) const;

        //! Executes an ioctl on an open device.
        /*!
        The device must be open.
        This function is overloaded for convenience.
        \param request request to pass to the ioctl command.
        \param arg additional argument to pass to the ioctl command.

        <b>Error codes:</b>
            - \b 0: Everything OK.
            - \b mrf_error::device_not_open: The device is not open.
            - \b mrf_error::ioctl_failed: Ioctl failed.
        */
        virtual int doIoctl(const int& request, void* arg) const;


        //! Reads from an open device.
        /*!
        The device must be open.
        Parameter list:
        \param address address of the register to read from.
        \return Register contents. Defaults to 0 if read fails.

        <b>Error codes:</b>
            - \b 0: Everything OK.
            - \b mrf_error::device_not_open: The device is not open.
            - \b mrf_error::read_failed: Could not read from device.

        <b>Implementation notes:</b>
        Should invoke doIoctl for ioctl.
        */
        virtual mrf::registertype read(const mrf::addresstype& address) const;


        //! Writes to an open device.
        /*!
        The device must be open.
        \param address Address of the register to write to.
        \param value Value to write.

        <b>Error codes:</b>
            - \b 0: Everything OK.
            - \b mrf_error::device_not_open: The device is not open.
            - \b mrf_error::write_failed: Could not write to device.

        <b>Implementation notes:</b>
        Should invoke doIoctl for ioctl.
        */
        virtual void write(const mrf::addresstype& address, const mrf::registertype& value) const;


        //! Reads an arbitrary amount of data from an open device.
        /*!
        The device must be open.
        \param address: Address of the register to read from.
        \param data: TMrfData structure which provides space for the data to be read.
        \return Amount of read data.

        <b>Error codes:</b>

        <b>Implementation notes:</b>
        */
        virtual u_int32_t readData(const mrf::addresstype& address, TMrfData& value) const;


        //! Writes an arbitrary amount of data to an open device.
        /*!
        The device must be open.
        Parameter list:
        \param address Address of the register to write to.
        \param data TMrfData structure storing the data to be written.
        \return Amount of written data.

        <b>Error codes:</b>

        <b>Implementation notes:</b>
        */
        virtual u_int32_t writeData(const mrf::addresstype& address, const TMrfData& value) const;



        //! Interrupt handling
        /*! These functions are not supported yet.
         */
        virtual void enableIRQ(const u_int32_t& mask) const {}
        virtual void disableIRQ(const u_int32_t& mask) const {}
        virtual u_int32_t waitforIRQ(const u_int32_t& mask) { return 0; }
        virtual u_int32_t getIRQ(const u_int32_t& mask) { return 0; }
        virtual u_int32_t waitforIRQFast(const u_int32_t& mask) const { return 0; }
        virtual u_int32_t getIRQFast(const u_int32_t& mask) const { return 0; }
        virtual void ackIRQFast(u_int32_t mask) const {}
        virtual const u_int32_t& getLastInterrupt() const { return 0; }


        //! Get currently assigned interface addresses
        /*!
          Get all the currently assigned interface addresses (like 192.168.0.2 on eth0).
          \return Vector of strings containing the addresses.
          */
        virtual std::map<std::string,std::string> getIfAddresses();



    private:

        //! Send some data to the sockets buffer
        /*!
          Send the data given as an 32 bit integer vector to the sockets buffer. The socket itself only sends 8 bit blocks.
          \param toSend The 32 bit integer vector to send.
          \return 0 if everything went well, -1 if an error occured.
          */
        virtual int _sendBuffer(std::vector<u_int32_t> toSend) const;

        //! Send a 32 bit integer to the sockets buffer
        /*!
          Send a 32 bit integer to the sockets buffer. This is an alias that calls _sendBuffer() with a vector.
          \param toSend The 32 bit integer to send.
          \return 0 if everything went well, -1 if an error occured.
          */
        virtual int _sendBuffer(const u_int32_t &toSend) const;

        //! Read the data coming into the buffer
        /*!
          Read the data into the TMrfData object given as a parameter.
          \param *data The pointer to the TMrfData object with the memory to write into.
          \return 0 if everything went well, -1 if an error occured, -2 if the request was timed out.
          */
        virtual int _readBuffer(char* const data, u_int32_t length, u_int32_t* wordcount = 0) const;



        //! Split connectionParameter into single strings
        /*!
          The connectionParameter is given as a single string of characters but containing four individual paramters:
          the IP and port for the source and destination.
          \param connectionParameter A string, containing the connection information
          \return Returns a vector with four strings: srcIP, srcPort, destIP, destPort
          */
        virtual std::vector<std::string> _splitConnectionParameter(const char *const connectionParameter);

        //! Create an UDP socket
        /*!
          Create the UDP socket that is used to transfer data
          \param destIP The IP to connect to.
          \param destPort The Port to connect to.
          \return True if successfull, False otherwise.
          */
        virtual bool _createUDPsocket(const char *const destIP, const char *const destPort);

        //! Bind the UDP socket to IP/Port
        /*!
          Bind an existing UDP socket to a device with a given IP and Port.
          \param srcIP The IP to bind to.
          \param srcPort The Port to bind to.
          \return True if successfull, False otherwise.
          */
        virtual bool _bindUDPsocket(const char *const srcIP, const char *const srcPort);

        //! Set a timeout for the UDP socket
        /*!
          \param timeoutSeconds The seconds for the new timeout setting.
          \param timeoutMicroseconds The micro seconds for the new timeout setting.
          \return True if successfull, False otherwise.
          */
        virtual bool _setUDPsocketTimeout(const int timeoutSeconds = 1, const int timeoutMicroseconds = 0);

        //! Checks a given IP address for validity
        /*!
          Performs a simple check on the given IP: do we have 4 integers in the range [0..254], seperated by dots?
          \param IP The IP address to check
          \return True for a valid IP address, False otherwise.
          */
        virtual bool _checkIP(const char *const IP) const;

        //! Checks a given port for validity
        /*!
          Performs a simple check on the given port: do we have a integer in the range [1024..65536]?
          \param port The port number to check
          \return True for a valid port, False otherwise.
          */
        virtual bool _checkPort(const char *const port) const;


        //! The socket ID for the communication to the testing device (e.g. ML605)
        int _UDPsocket;

        //! The address of the testing device (e.g. ML605), that the socket should link to
        struct addrinfo *_UDPaddr;

        //! Poll information from the buffer and run into a timeout, if none are available
        struct pollfd _UDPpollInfo;

        //! The answer of the poll request
        int _UDPpollInfoResult;

        //! The timeout for waiting of receiving data in ms
        const static int _UDPrecvTimeout = 500;

        //! A generic TMrfData object to not reserve memory for every control access
        TMrfData _genericBuffer;

        const static bool _verbose = false;
};

#endif // __MRFGAL_UDP_H__
