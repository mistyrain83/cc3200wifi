
// Standard includes
#include <stdlib.h>
#include <string.h>

// simplelink includes 
#include "simplelink.h"
#include "wlan.h"

// driverlib includes 
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart.h"
#include "utils.h"

// Common interface includes
#include "gpio_if.h"
#include "uart_if.h"
#include "common.h"

// Application specific status/error codes
typedef enum{
    // Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
    TCP_CLIENT_FAILED = -0x7D0,
    TCP_SERVER_FAILED = TCP_CLIENT_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = TCP_SERVER_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

//****************************************************************************
//
//! \brief Opening a TCP client side socket and sending data
//!
//! This function opens a TCP socket and tries to connect to a Server IP_ADDR
//!    waiting on port PORT_NUM.
//!    If the socket connection is successful then the function will send 1000
//! TCP packets to the server.
//!
//! \param[in]      port number on which the server will be listening on
//!
//! \return    0 on success, -1 on Error.
//
//****************************************************************************
int BsdTcpClient(unsigned int ulDestinationIp, unsigned short usPort)
{
    SlSockAddrIn_t  sAddr;
    int             iAddrSize;
    int             iSockID;
    int             iStatus;


    //filling the TCP server socket address
    sAddr.sin_family = SL_AF_INET;
    sAddr.sin_port = sl_Htons((unsigned short)usPort);
    sAddr.sin_addr.s_addr = sl_Htonl((unsigned int)ulDestinationIp);

    iAddrSize = sizeof(SlSockAddrIn_t);

    // creating a TCP socket
    iSockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if( iSockID < 0 )
    {
        ASSERT_ON_ERROR(TCP_CLIENT_FAILED);
    }

    // connecting to TCP server
    iStatus = sl_Connect(iSockID, ( SlSockAddr_t *)&sAddr, iAddrSize);
    if( iStatus < 0 )
    {
        // error
        ASSERT_ON_ERROR(sl_Close(iSockID));
        ASSERT_ON_ERROR(TCP_CLIENT_FAILED);
    }

    return iSockID;
}

//****************************************************************************
//
//! \brief Opening a TCP server side socket and receiving data
//!
//! This function opens a TCP socket in Listen mode and waits for an incoming
//!    TCP connection.
//! If a socket connection is established then the function will try to read
//!    1000 TCP packets from the connected client.
//!
//! \param[in] port number on which the server will be listening on
//!
//! \return     0 on success, -1 on error.
//!
//! \note   This function will wait for an incoming connection till
//!                     one is established
//
//****************************************************************************
int BsdTcpServer(int *iSockID, unsigned short usPort)
{
    SlSockAddrIn_t  sLocalAddr;
    int             iAddrSize;
    int             iStatus;
    long            lNonBlocking = 1;

    //filling the TCP server socket address
    sLocalAddr.sin_family = SL_AF_INET;
    sLocalAddr.sin_port = sl_Htons((unsigned short)usPort);
    sLocalAddr.sin_addr.s_addr = 0;

    // creating a TCP socket
    *iSockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if( *iSockID < 0 )
    {
        // error
        ASSERT_ON_ERROR(TCP_SERVER_FAILED);
    }

    iAddrSize = sizeof(SlSockAddrIn_t);

    // binding the TCP socket to the TCP server address
    iStatus = sl_Bind(*iSockID, (SlSockAddr_t *)&sLocalAddr, iAddrSize);
    if( iStatus < 0 )
    {
        // error
        ASSERT_ON_ERROR(sl_Close(*iSockID));
        ASSERT_ON_ERROR(TCP_SERVER_FAILED);
    }

    // putting the socket for listening to the incoming TCP connection
    iStatus = sl_Listen(*iSockID, 0);
    if( iStatus < 0 )
    {
        ASSERT_ON_ERROR(sl_Close(*iSockID));
        ASSERT_ON_ERROR(TCP_SERVER_FAILED);
    }

    // setting socket option to make the socket as non blocking
    iStatus = sl_SetSockOpt(*iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, 
                            &lNonBlocking, sizeof(lNonBlocking));
    if( iStatus < 0 )
    {
        ASSERT_ON_ERROR(sl_Close(*iSockID));
        ASSERT_ON_ERROR(TCP_SERVER_FAILED);
    }

    return SUCCESS;
}

