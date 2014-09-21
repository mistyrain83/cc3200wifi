
#ifndef __PWMOUT_H__
#define __PWMOUT_H__

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

void UpdateDutyCycle(unsigned long ulBase, unsigned long ulTimer, unsigned char ucLevel);
void SetupTimerPWMMode(unsigned long ulBase, unsigned long ulTimer, unsigned long ulConfig, unsigned char ucInvert);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __PWMOUT_H__
