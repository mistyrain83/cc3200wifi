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

long WriteFileToDevice(unsigned char *pFileName, unsigned char *ip, unsigned char *port);

long ReadFileFromDevice(unsigned char *pFileName, unsigned char *ip, unsigned char *port);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif //__FILEOPERATOR__H__