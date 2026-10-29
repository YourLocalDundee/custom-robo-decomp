/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, src/dsp/dsp_debug.c */ #include <dolphin/dsp.h>

#include "__dsp.h"

void __DSP_debug_printf(const char* fmt, ...) {}

DSPTaskInfo* __DSPGetCurrentTask(void) {
    return __DSP_curr_task;
}
