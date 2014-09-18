
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

#define CONFIG_FILE_LEN_MAX 56

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
//!  -open a user file for writing
//!  -write "Old MacDonalds" child song 37 times to get just below a 64KB file
//!  -close the user file
//!
//!  /param[out] ulToken : file token
//!  /param[out] lFileHandle : file handle
//!
//!  /return  0:Success, -ve: failure
//
//*****************************************************************************
long WriteFileToDevice(unsigned char *pFileName, unsigned char *ip, unsigned char *port)
{
    long lRetVal = -1;
    int iLoopCnt = 0;
	long lFileHandle = -1;
	unsigned char str[CONFIG_FILE_LEN_MAX];

    //
    //  create a user file
    //
    lRetVal = sl_FsOpen((unsigned char *)pFileName,
                FS_MODE_OPEN_CREATE(CONFIG_FILE_LEN_MAX, \
                          _FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE),
                        NULL,
                        &lFileHandle);
    if(lRetVal < 0)
    {
        //
        // File may already be created
        //
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(lRetVal);
    }
    else
    {
        //
        // close the user file
        //
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        if (SL_RET_CODE_OK != lRetVal)
        {
            ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
        }
    }
    
    //
    //  open a user file for writing
    //
    lRetVal = sl_FsOpen((unsigned char *)pFileName,
                        FS_MODE_OPEN_WRITE, 
                        NULL,
                        &lFileHandle);
    if(lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(FILE_OPEN_WRITE_FAILED);
    }
    
    //
    // write "Old MacDonalds" child song as many times to get just below a 64KB file
    //
    sprintf(str, "i%sp%se", ip, port);
        lRetVal = sl_FsWrite(lFileHandle,
                    0, 
                    (unsigned char *)str, sizeof(str));
        if (lRetVal < 0)
        {
            lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
            ASSERT_ON_ERROR(FILE_WRITE_FAILED);
        }
    
    
    //
    // close the user file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
    }

    return SUCCESS;
}

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
long ReadFileFromDevice(unsigned char *pFileName, unsigned char *ip, unsigned char *port)
{
        long lRetVal = -1;
    int iLoopCnt = 0;
	long lFileHandle;

	unsigned char c;
	unsigned int index = 0;
	unsigned int offset = 0;

    //
    // open a user file for reading
    //
    lRetVal = sl_FsOpen((unsigned char *)pFileName,
                        FS_MODE_OPEN_READ,
                        NULL,
                        &lFileHandle);
    if(lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(FILE_OPEN_READ_FAILED);
    }

    //
    // read the data and compare with the stored buffer
    //
    
        lRetVal = sl_FsRead(lFileHandle,
                    0,
                     &c, 1);
        if ((lRetVal < 0) || (lRetVal != 1))
        {
            lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
            ASSERT_ON_ERROR(FILE_READ_FAILED);
        }
		offset++;
        UART_PRINT("read 0x%x\n\t", c);
		
		if(c == 0x69)
		{
			while(c != 0x70)
			{
				lRetVal = sl_FsRead(lFileHandle,
						offset,
						 &c, 1);
				if ((lRetVal < 0) || (lRetVal != 1))
				{
					lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
					ASSERT_ON_ERROR(FILE_READ_FAILED);
				}
				
				ip[index] = c;
				index++;
				offset++;
			}
			ip[index-1] = 0;
			
			UART_PRINT("ip = %s\n\t", ip);
			index = 0;
			while(c != 0x65)
			{
				lRetVal = sl_FsRead(lFileHandle,
						offset,
						 &c, 1);
				if ((lRetVal < 0) || (lRetVal != 1))
				{
					lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
					ASSERT_ON_ERROR(FILE_READ_FAILED);
				}
				
				port[index] = c;
				index++;
				offset++;
				//UART_PRINT("c = 0x%x\n\t", c);
			}
			port[index-1] = 0;
			
			UART_PRINT("port = %s\n\t", port);
			index = 0;
		}
    

    //
    // close the user file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
    }

    return SUCCESS;
}
