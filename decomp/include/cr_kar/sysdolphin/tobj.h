/* Custom Robo override of doldecomp/kar src/sysdolphin/tobj.h (step 61 iteration 7): found before include/kar
 * (decomp/configure.py CR_OBJECT_INCLUDES), so kar's mobj.c compiles unchanged. Names only: kar's mobj.c calls
 * HSD_TObjAddAnim(mobj->tobj, ...) in HSD_MObjAddAnim; the function at that call site (0x80186B44, 0x154) is
 * HSD_TObjAddAnimAll: wowjinxy/KAR's HSD_TObjAddAnimAll (the TObj list loop with TObjAddAnim inline) compiled here
 * equals it, the wowjinxy KAR map has HSD_TObjAddAnimAll at 0x154, and Melee's HSD_MObjAddAnim calls
 * HSD_TObjAddAnimAll there. The header itself is the kar copy, unchanged. */
#define HSD_TObjAddAnim HSD_TObjAddAnimAll
#include "../../kar/sysdolphin/tobj.h"
