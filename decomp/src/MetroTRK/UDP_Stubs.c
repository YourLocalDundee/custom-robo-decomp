/* Custom Robo: from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/metrotrk.c */ /* Custom Robo: only the UDP_Stubs.c functions of that file (udp_cc_*), in reverse order: the TRK flags' deferred inlining emits the last first, giving main.dol's order, and its DSError values */
typedef int DSError;
enum { kNoError = 0, kUARTError = -0x0001 };

__declspec(weak) DSError udp_cc_initialize(void* ptr, void* callback)
{
    return kUARTError;
}

__declspec(weak) DSError udp_cc_shutdown(void)
{
    return kUARTError;
}

__declspec(weak) DSError udp_cc_open(void)
{
    return kUARTError;
}

__declspec(weak) DSError udp_cc_close(void)
{
    return kUARTError;
}

__declspec(weak) DSError udp_cc_read(void* data, unsigned long limit)
{
    return kNoError;
}

__declspec(weak) DSError udp_cc_write(const void* data, unsigned long length)
{
    return kNoError;
}

__declspec(weak) DSError udp_cc_peek(void)
{
    return kNoError;
}

__declspec(weak) DSError udp_cc_pre_continue(void)
{
    return kUARTError;
}

__declspec(weak) DSError udp_cc_post_stop(void)
{
    return kUARTError;
}
