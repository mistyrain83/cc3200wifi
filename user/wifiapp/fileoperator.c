
#include <stdlib.h>
#include <string.h>

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"

#include "common.h"
#ifndef NOTERM
#include "uart_if.h"
#endif

#include "fileoperator.h"


/* Application specific status/error codes */
typedef enum{
    // Choosing this number to avoid overlap w/ host-driver's error codes
    FILE_ALREADY_EXIST = -0x7D0,
    FILE_CLOSE_ERROR = FILE_ALREADY_EXIST - 1,
    FILE_NOT_MATCHED = FILE_CLOSE_ERROR - 1,
    FILE_OPEN_READ_FAILED = FILE_NOT_MATCHED - 1,
    FILE_OPEN_WRITE_FAILED = FILE_OPEN_READ_FAILED -1,
    FILE_READ_FAILED = FILE_OPEN_WRITE_FAILED - 1,
    FILE_WRITE_FAILED = FILE_READ_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

//*****************************************************************************
//
//!  This funtion includes the following steps:
//!    -open the user file for reading
//!    -read the data and compare with the stored buffer
//!    -close the user file
//!
//!  /param[in] ulToken : file token
//!  /param[in] lFileHandle : file handle
//!
//!  /return 0: success, -ve:failure
//
//*****************************************************************************
T_IP_CONFIG ReadConfigFromDevice(unsigned char *pFileName)
{
	T_IP_CONFIG tIpConfig;
    long lRetVal = -1;
    int iLoopCnt = 0;
	unsigned long ulToken;
	long lFileHandle;
	unsigned char cIPNum = 0;
	unsigned char cPortNum = 0;

    //
    // open a user file for reading
    //
    lRetVal = sl_FsOpen((unsigned char *)"config.txt",
                        FS_MODE_OPEN_READ,
                        &ulToken,
                        &lFileHandle);
    if(lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        
    }

    //
    // read the data
    //
	lRetVal = sl_FsRead(lFileHandle,
				0,
				&cIPNum, 1);
	if ((lRetVal < 0) || (lRetVal != 1))
	{
		lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		
	}
	UART_PRINT("cIpNum = %d\n\r", cIPNum);
	lRetVal = sl_FsRead(lFileHandle,
				1,
				 tIpConfig.ipAddress, cIPNum);
	if ((lRetVal < 0) || (lRetVal != cIPNum))
	{
		lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		
	}
	tIpConfig.ipAddress[cIPNum] = 0;
	UART_PRINT("ipAddress = %s\n\r", tIpConfig.ipAddress);
	lRetVal = sl_FsRead(lFileHandle,
				(1+cIPNum),
				 &cPortNum, 1);
	if ((lRetVal < 0) || (lRetVal != 1))
	{
		lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		
	}
	UART_PRINT("cPortNum = %d\n\r", cPortNum);
	lRetVal = sl_FsRead(lFileHandle,
				(1+cIPNum+1),
				 tIpConfig.port, cPortNum);
	if ((lRetVal < 0) || (lRetVal != cPortNum))
	{
		lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		
	}
        tIpConfig.port[cPortNum] = 0;
	UART_PRINT("ipAddress = %s\n\r", tIpConfig.port);

    //
    // close the user file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        
    }

    return tIpConfig;
}
