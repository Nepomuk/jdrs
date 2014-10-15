#include "mrfgal_udp.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

#include <netdb.h>
#include <netinet/in.h>		// INET constants and stuff
#include <arpa/inet.h>		// IP address conversion stuff
#include <sys/types.h>
#include <sys/socket.h>		// Socket specific definitions
#include <ifaddrs.h>        // get interface addresses
//#include <fcntl.h>          // manipulate a file discriptor
#include <sys/poll.h>
#include <unistd.h>

#include "stringseparator.h"


TMrfGal_Udp::TMrfGal_Udp()
    : _UDPsocket(-1)
{
    _genericBuffer.setNumWords(MAX_BLOCKS_PER_UDP_PACKAGE);
}

TMrfGal_Udp::~TMrfGal_Udp()
{
    closeDevice();
}

void TMrfGal_Udp::openDevice(const char *const connectionParameter)
{
    if (!deviceIsOpen()) {
        // split the string of connection parameter
        std::vector<std::string> connParVec = _splitConnectionParameter(connectionParameter);

        if ( connParVec.size() != 4 ) {
            // something went wrong (error msg is handled in splitConnectionParameter)
            return;
        }
        const char *srcIP     = connParVec[0].c_str();
        const char *srcPort   = connParVec[1].c_str();
        const char *destIP    = connParVec[2].c_str();
        const char *destPort  = connParVec[3].c_str();

        // create the UDP socket
        if ( !_createUDPsocket(destIP, destPort) ) return;

        // bind the socket to a device
        if ( !_bindUDPsocket(srcIP, srcPort) ) return;

        // set a timeout to the socket to prevent the program from being crashed
        //if ( !_setUDPsocketTimeout(1,0) ) return;
        //fcntl(_UDPsocket, F_SETFL, O_NONBLOCK);

        // the device should now be open
        if (!deviceIsOpen()) {
            std::cerr << "OPEN failed: " << strerror(errno) << std::endl;
            errcode |= mrf_error::device_not_open;
        }
        else {
            errcode = mrf_error::success;
        }

    }
    else {
        errcode |= mrf_error::already_open;
    }
}

void TMrfGal_Udp::closeDevice()
{
    if ( deviceIsOpen() ) {
        if ( close(_UDPsocket) == 0 ) {
            errcode = mrf_error::success;
        }
        else {
            std::cerr << "CLOSE failed: " << strerror(errno) << std::endl;
            errcode |= mrf_error::close_failed;
        }
        _UDPsocket = -1;
    }
    else {
        errcode |= mrf_error::device_not_open;
    }
}

bool TMrfGal_Udp::deviceIsOpen() const
{
    // socket has no ID
    if ( _UDPsocket <= 0 )
        return false;

    return true;
}

bool TMrfGal_Udp::deviceIsOnline() const
{
    if ( !deviceIsOpen() )
        return false;

    _sendBuffer(udpDataFlag::ping);

    u_int32_t response = udpDataFlag::nothing;
    if ( _readBuffer((char*)&response, sizeof(u_int32_t)) != 0 ||
         response != udpDataFlag::pong ) {
        return false;
    } else {
        return true;
    }
}

int TMrfGal_Udp::doIoctl(const int &request) const
{
    // actually I don't know what to do with that
//    return _sendBuffer(request);
    return 0;
}

int TMrfGal_Udp::doIoctl(const int &requestType, void *arg) const
{
    if ( (u_int32_t)requestType != udpDataFlag::bulkRead )
        return -1;

    // put together the request to send
    std::vector<u_int32_t> toSend;
    udpRequest *req = (udpRequest*)arg;

    // our protocol supports only packages up to a certain size
    if ( req->wordcount > MAX_BLOCKS_PER_UDP_PACKAGE )
        req->wordcount = MAX_BLOCKS_PER_UDP_PACKAGE;

    toSend.push_back(req->dataFlag);
    toSend.push_back(req->registerAddress);
    toSend.push_back(req->wordcount);

    if ( _sendBuffer(toSend) != 0 ) {
        errcode = mrf_error::ioctl_failed;
        return -1;
    }

    // read the answer that we get from the board
    if ( _readBuffer(req->memoryAddress, req->wordcount*4 + 4, &(req->wordcount)) != 0 ) {
        errcode = mrf_error::read_failed;
    }

    return 0;
}

mrf::registertype TMrfGal_Udp::read(const mrf::addresstype& address) const
{
    std::vector<u_int32_t> toSend;
    toSend.push_back(udpDataFlag::registerRead);
    toSend.push_back(address);

    if ( _sendBuffer(toSend) != 0 ) {
        errcode = mrf_error::ioctl_failed;
        return 0;
    }

    u_int32_t response[2] = {0};
    if ( _readBuffer((char*)&response, sizeof(u_int32_t)*2) != 0 ||
        response[0] != udpDataFlag::registerRead ) {
        errcode = mrf_error::read_failed;
    }

//    if ( dataStream.size() < 2 ) {
//        std::cout << "size = " << dataStream.size() << std::endl;
//        std::cout << "dataFlag = " << (dataStream[0] >> 3*8) << std::endl;
//        return ret;
//    }

    errcode = mrf_error::success;
    return response[1];
}


void TMrfGal_Udp::write(const mrf::addresstype& address, const mrf::registertype& value) const
{
    std::vector<u_int32_t> toSend;
    toSend.push_back(udpDataFlag::registerWrite);
    toSend.push_back(address);
    toSend.push_back(value);

    if ( _sendBuffer(toSend) != 0 ) {
        errcode = mrf_error::ioctl_failed;
        return;
    }

    u_int32_t response = udpDataFlag::nothing;
    if ( _readBuffer((char*)&response, sizeof(u_int32_t)) != 0 ||
         response != udpDataFlag::check ) {
        errcode = mrf_error::read_failed;
    }

    return;
}


u_int32_t TMrfGal_Udp::writeData(const mrf::addresstype& address, const TMrfData& data) const
{
    if (deviceIsOpen()) {
        u_int32_t i;
        for (i = 0; i < data.getNumWords(); ++i) {
            write(address, data.getWord(i));
            if (!(lastActionSuccessful())) {
                errcode |= mrf_error::write_data_failed;
                break;
            }
        }
        return i;
    } else {
        errcode |= mrf_error::device_not_open;
        return 0;
    }
}

u_int32_t TMrfGal_Udp::readData(const mrf::addresstype& address, TMrfData& data) const
{
    if (deviceIsOpen()) {
        u_int32_t i;
        for (i = 0; i < data.getNumWords(); ++i) {
            data.setWord(i, read(address));
            if (!(lastActionSuccessful())) {
                errcode |= mrf_error::read_data_failed;
                break;
            }
        }
        return i;
    } else {
        errcode |= mrf_error::device_not_open;
        return 0;
    }
}


std::map<std::string,std::string> TMrfGal_Udp::getIfAddresses() {
    std::map<std::string,std::string> addressMap;
    std::string currentAddress, currentInterfaceName;

    // check if open and stuff

    struct ifaddrs *interfaces, *currentInterface;
    int s;
    char ifHostAddress[NI_MAXHOST];

    if ( getifaddrs(&interfaces) == -1 ) {
        std::cerr << "GETIFADDR failed: " << strerror(errno) << std::endl;
        errcode |= mrf_error::open_failed;
    }

    // Walk through linked list, maintaining head pointer so we can free list later
    for ( currentInterface = interfaces; currentInterface != NULL; currentInterface = currentInterface->ifa_next ) {
        if ( currentInterface->ifa_addr == NULL )
            continue;

        if ( currentInterface->ifa_addr->sa_family != AF_INET )
            continue;

        // get the address from the current device
        s = getnameinfo(
            currentInterface->ifa_addr,
            sizeof(struct sockaddr_in),
            ifHostAddress,
            NI_MAXHOST, NULL, 0, NI_NUMERICHOST
        );

        if (s != 0) {
            std::cerr << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
            return addressMap;
        }

        currentAddress = std::string(ifHostAddress);
        currentInterfaceName = std::string(currentInterface->ifa_name);
        if ( addressMap.find(currentAddress) != addressMap.end() )
            continue;

        std::pair<std::string,std::string> currentPair(currentAddress, currentInterfaceName);
        addressMap.insert(currentPair);
    }

    freeifaddrs(interfaces);
    return addressMap;
}



// -------------------------
//   private functions
// -------------------------

int TMrfGal_Udp::_sendBuffer(const u_int32_t &toSend) const
{
    std::vector<u_int32_t> toSendVector;
    toSendVector.push_back(toSend);

    // call the main function which accepts a vector of bytes
    return _sendBuffer(toSendVector);
}

int TMrfGal_Udp::_sendBuffer(std::vector<u_int32_t> toSend) const
{
    if ( !deviceIsOpen() ) {
        std::cerr << "SEND failed, device not open." << std::endl;
        errcode |= mrf_error::device_not_open;
        return -1;
    }

    size_t bytesSent = 0;

    // since we are sending 32 bit integers and ethernet transfers 8 bit at once,
    // we need to consider 32 bit (= 4 Byte) blocks
    size_t nBlocksToSend = (size_t)toSend.size();
    if ( nBlocksToSend > MAX_BLOCKS_PER_UDP_PACKAGE )
        nBlocksToSend = MAX_BLOCKS_PER_UDP_PACKAGE;
    const size_t nBytesToSend = nBlocksToSend*4;

    // put the byte order of the single entries to a host independant standard
    // todo: check if this is relevant to the transfer speed
    // maybe: #if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
    for ( unsigned int i = 0; i < toSend.size(); i++ ) {
        toSend[i] = htonl(toSend[i]);
    }

    // do the actual sending
    bytesSent = sendto( _UDPsocket,             // UDP socket
                        (char*)&toSend[0],      // the pointer to the first block
                        nBytesToSend,			// The datagram length
                        0,						// Flags: no options
                        _UDPaddr->ai_addr,      // destination address
                        _UDPaddr->ai_addrlen);  // destination address length

    if ( (int)bytesSent < 0 ) {
        std::cerr << "SENDING of " << nBytesToSend << " failed." << std::endl;
        errcode |= mrf_error::write_failed;
        return -1;
    }


    if ( _verbose ) {
        std::cout << "Sending..." << std::endl;
        char out[64];
        unsigned char *byte;
        for ( int i = 0; i < toSend.size(); i++ ) {
            byte = (unsigned char*)&toSend[i];
            sprintf( out, "word %d:  ", i );
            for ( int j = 0; j < 4; j++ ) { sprintf( out, "%s %02x", out, *(byte+j) ); }
            std::cout << out << std::endl;
        }
    }

    return 0;
}

int TMrfGal_Udp::_readBuffer(char* const dataStartAddress, u_int32_t length, u_int32_t* wordcount) const
{

    // TODO! remove this stuff! We should be directly writing to the MrfData
    // buffer for receiving data and final byte vector
    //char recvBuffer[MAX_BLOCKS_PER_UDP_PACKAGE*4] = {0};
    if ( length > MAX_BYTES_PER_UDP_PACKAGE )
        length = MAX_BYTES_PER_UDP_PACKAGE;

    if ( !deviceIsOpen() ) {
        std::cerr << "SEND failed, device not open." << std::endl;
        errcode |= mrf_error::device_not_open;
        return -1;
    }

    // first check, if data are available on the socket
    // todo: this has to be done only once
    //_UDPpollInfoResult = poll(&_UDPpollInfo, 1, _UDPrecvTimeout);
    struct pollfd UDPpollInfo;
    UDPpollInfo.fd = _UDPsocket;
    UDPpollInfo.events = POLLIN;
    int UDPpollInfoResult = poll(&UDPpollInfo, 1, _UDPrecvTimeout);

    if ( UDPpollInfoResult == -1 ) {
        // an error occured
        std::cerr << "POLL failed: " << strerror(errno) << std::endl;
        return -1;
    } else if ( UDPpollInfoResult == 0 ) {
        // timeout
        std::cerr << "POLL timeout" << std::endl;
        return -2;
    } else {
        // OK, data are available...

        // receiving address information
        // todo: do it once?
        sockaddr_storage recvAddr;
        socklen_t recvAddrLen = sizeof(recvAddr);

        // bytes actually received
        // todo: reserve memory once?
        size_t nBytesRecv = 0;

        const size_t recvLength = MAX_BYTES_PER_UDP_PACKAGE;
        char recv[recvLength];

        // receive the data from the buffer
        nBytesRecv = recvfrom(
            _UDPsocket,             // UDP socket with ML605
            &recv,
            recvLength,
//            dataStartAddress,                   // Pointer to the datagram result buffer
//            length,                 // The max. datagram length in bytes
            0,                      // Flags: no options
            (sockaddr*)&recvAddr,   // receiving address
            &recvAddrLen            // receiving address length
        );

        if ( (int)nBytesRecv < 0 ) {
            std::cerr << "RECEIVING failed: " << strerror(errno) << std::endl;
            return -1;
        }


        char out[64];
        unsigned char *byte;
        if ( _verbose ) {
            std::cout << "Reading..." << std::endl;
        }

        // check that the returning 32 bit integers have the correct byte order
        u_int32_t *recvSingleBlock, *returnSingleBlock;
        for ( unsigned int i = 0; i < length && i < nBytesRecv; i += 4 ) {
//            recvSingleBlock = (u_int32_t*)(dataStartAddress+i);
//            *recvSingleBlock = ntohl(*recvSingleBlock);
            recvSingleBlock = (u_int32_t*)(&recv[i]);
            returnSingleBlock = (u_int32_t*)(dataStartAddress+i);
            *returnSingleBlock = ntohl(*recvSingleBlock);

            if ( _verbose ) {
                sprintf( out, "word %d:  ", i/4 );
                byte = (unsigned char*)(recvSingleBlock);
                for ( int j = 0; j < 4; j++ ) { sprintf( out, "%s %02x", out, *(byte+j) ); }
                std::cout << out << std::endl;
            }
        }

        // if an address for the wordcount is given, set it to the read words
        if ( wordcount != 0 ) {
            *wordcount = ((nBytesRecv-4)/4);
        }

        return 0;
    }
}

std::vector<std::string> TMrfGal_Udp::_splitConnectionParameter(const char *const connectionParameter)
{
    std::vector<std::string> emptyVec;

    // split the string of connection parameter
    //  example connectionParameter:
    //  connectionParameter = "192.168.0.1;42000;192.168.0.23;52000"
    std::vector<std::string> connParVec = StringSeparator(std::string(connectionParameter), ",").GetStringVector();

    if ( connParVec.size() != 4 ) {
        std::cerr << "OPEN failed, connectionParameter '" << connectionParameter << "' has wrong format. Should be something like \"192.168.0.1,42000,192.168.0.5,52000\"." << std::endl;
        errcode |= mrf_error::open_failed;
        return emptyVec;
    }

    // check for plausebality
    if ( !_checkIP(connParVec[0].c_str()) ) {
        std::cerr << "OPEN failed, source IP '" << connParVec[0] << "' wrong." << std::endl;
        errcode |= mrf_error::open_failed;
        return emptyVec;
    }
    if ( !_checkPort(connParVec[1].c_str()) ) {
        std::cerr << "OPEN failed, source port '" << connParVec[1] << "' wrong." << std::endl;
        errcode |= mrf_error::open_failed;
        return emptyVec;
    }
    if ( !_checkIP(connParVec[2].c_str()) ) {
        std::cerr << "OPEN failed, destination IP '" << connParVec[2] << "' wrong." << std::endl;
        errcode |= mrf_error::open_failed;
        return emptyVec;
    }
    if ( !_checkPort(connParVec[3].c_str()) ) {
        std::cerr << "OPEN failed, destination port '" << connParVec[3] << "' wrong." << std::endl;
        errcode |= mrf_error::open_failed;
        return emptyVec;
    }

    return connParVec;
}

bool TMrfGal_Udp::_createUDPsocket(const char *const destIP, const char *const destPort)
{
    // set the parameters for the connection (where to connect to)
    struct addrinfo addr;
    memset(&addr, 0, sizeof(addr));
    addr.ai_family      = AF_INET;  // IPv4
    addr.ai_socktype    = SOCK_DGRAM;

    // check with a DNS loopup
    if ( getaddrinfo(destIP, destPort, &addr, &_UDPaddr) != 0 ) {
        std::cerr << "OPEN failed, couldn't get address information: " << strerror(errno) << std::endl;
        errcode |= mrf_error::open_failed;
        return false;
    }

    // create the socket with the connection information
    _UDPsocket = socket(_UDPaddr->ai_family,
                        _UDPaddr->ai_socktype,
                        _UDPaddr->ai_protocol);

    if ( _UDPsocket == -1 ) {
        std::cerr << "OPEN failed, socket could not be created: " << strerror(errno) << std::endl;
        errcode |= mrf_error::open_failed;
        return false;
    }

    // since we want to poll information, we have to set some information about the socket
    _UDPpollInfo.fd = _UDPsocket;
    _UDPpollInfo.events = POLLIN;

    return true;
}

bool TMrfGal_Udp::_bindUDPsocket(const char *const srcIP, const char *const srcPort)
{
    // socket is not open
    if ( _UDPsocket == -1 )
        return false;

    // now that we have an active socket, bind it to a specific local IP address
    struct sockaddr_in src_addr;
    memset((char *)&src_addr, 0, sizeof(src_addr));

    src_addr.sin_family = AF_INET;
    src_addr.sin_port = atoi(srcPort);
    inet_aton(srcIP, &src_addr.sin_addr);

    // bind the socket
    if ( bind(_UDPsocket, (sockaddr*)&src_addr, sizeof(src_addr)) == -1 ) {
        std::cerr << "OPEN failed, socket could not be bound to IP: " << strerror(errno) << std::endl;
        errcode |= mrf_error::open_failed;
        close(_UDPsocket);
        return false;
    }

    return true;
}

bool TMrfGal_Udp::_setUDPsocketTimeout(const int timeoutSeconds, const int timeoutMicroseconds)
{
    // socket is not open
    if ( _UDPsocket == -1 )
        return false;

    struct timeval tv;
    tv.tv_sec = ( timeoutSeconds < 0 ) ? 1 : timeoutSeconds;
    tv.tv_sec = ( timeoutMicroseconds < 0 ) ? 0 : timeoutMicroseconds;

    if ( setsockopt(_UDPsocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) == -1 ) {
        std::cerr << "SETSOCKOPT failed, could not modify the socket's timeout. " << strerror(errno) << std::endl;
        errcode |= mrf_error::open_failed;
        return false;
    }

    return true;
}

bool TMrfGal_Udp::_checkIP(const char *const IP) const
{
    // use the inet.h function to check for a valid IP address
    struct in_addr tmp;
    if ( inet_pton(AF_INET, IP, &(tmp.s_addr)) != 1 ) {
        return false;
    }

    return true;
}


bool TMrfGal_Udp::_checkPort(const char *const port) const
{
    int port_int = atoi(port);
    if ( port_int < 1024 || port_int > 65536 ) {
        return false;
    }

    return true;
}
