/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/include/dolphin/os/OSFst.h */ #ifndef _DOLPHIN_OSFST_H
#define _DOLPHIN_OSFST_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef struct OSFstEntry
{
    int entryNum;
    int nextEntryNum;
    char *fileNameMaybe;
} OSFstEntry;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif