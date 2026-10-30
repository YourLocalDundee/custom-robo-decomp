/* Custom Robo override of doldecomp/kar src/sysdolphin/wobj.h (step 61): found before include/kar (decomp/configure.py
 * CR_OBJECT_INCLUDES). kar's wobj.c calls the WObj class info by its KAR address, lbl_805041C8; its own assert string
 * ("hsdIsDescendantOf(info, &hsdWObj)"), Melee wobj.c and the wowjinxy KAR map name it hsdWObj, and so does
 * symbols/symbols.txt (0x8024CE00), so the definition gets that name here. kar's wobj.c names its parent class info
 * objInfo (WObjInfoInit); at that site main.dol has hsdObj (0x8024D050, object.c, linked from C), the name the wowjinxy
 * KAR source uses there. The header itself is kar's, unchanged. */
#define lbl_805041C8 hsdWObj
#define objInfo hsdObj
#include "../kar/sysdolphin/wobj.h"
