/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/critical_regions.h */ #ifndef _MSL_COMMON_CRITICAL_REGIONS_H
#define _MSL_COMMON_CRITICAL_REGIONS_H

#ifdef __cplusplus
extern "C" {
#endif

enum critical_regions {
	atexit_funcs_access,
	malloc_pool_access,
	stdin_access,
	stdout_access,
	stderr_access,
	files_access,
	console_status_access,
	signal_funcs_access,
	thread_access,
	num_critical_regions
};

void __init_critical_regions(void);
void __kill_critical_regions(void);
void __begin_critical_region(int region);
void __end_critical_region(int region);

#ifdef __cplusplus
}
#endif

#endif
