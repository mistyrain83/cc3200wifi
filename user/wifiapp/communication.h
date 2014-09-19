
#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

int BsdTcpClient(unsigned int ulDestinationIp, unsigned short usPort);
int BsdTcpServer(int *iSockID, unsigned short usPort);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __COMMUNICATION_H__
