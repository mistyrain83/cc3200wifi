#ifndef __FILEOPERATOR__H__
#define __FILEOPERATOR__H__


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

#define IP_ADDRESS_LEN_MAX 15
#define PORT_LEN_MAX 4

typedef struct 
{
	unsigned char ipAddress[IP_ADDRESS_LEN_MAX];
	unsigned char port[PORT_LEN_MAX];
}T_IP_CONFIG;

T_IP_CONFIG ReadConfigFromDevice(unsigned char *pFileName);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif //__FILEOPERATOR__H__