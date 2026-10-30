/* Custom Robo override of doldecomp/kar src/sysdolphin/wobj.h (step 61): found before include/kar (decomp/configure.py
 * CR_OBJECT_INCLUDES). kar's wobj.c calls the WObj class info by its KAR address, lbl_805041C8; its own assert string
 * ("hsdIsDescendantOf(info, &hsdWObj)"), Melee wobj.c and the wowjinxy KAR map name it hsdWObj, and so does
 * symbols/symbols.txt (0x8024CE00), so the definition gets that name here. The header itself is kar's, unchanged. */
#define lbl_805041C8 hsdWObj
#include "../kar/sysdolphin/wobj.h"
