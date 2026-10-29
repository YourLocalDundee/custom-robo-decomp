/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/os/OSStopwatch.h */ #ifndef _DOLPHIN_OSSTOPWATCH_H_
#define _DOLPHIN_OSSTOPWATCH_H_

struct OSStopwatch {
    char* name;
    long long total;
    unsigned long hits;
    long long min;
    long long max;
    long long last;
    int running;
};

void OSInitStopwatch(struct OSStopwatch* sw, char* name);
void OSStartStopwatch(struct OSStopwatch* sw);
void OSStopStopwatch(struct OSStopwatch* sw);
long long OSCheckStopwatch(struct OSStopwatch* sw);
void OSResetStopwatch(struct OSStopwatch* sw);
void OSDumpStopwatch(struct OSStopwatch* sw);

#endif // _DOLPHIN_OSSTOPWATCH_H_
