/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/PowerPC_EABI_Support/src/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_log.c */ 
#include "fdlibm.h"

double log(double x)
{
    return __ieee754_log(x);
}