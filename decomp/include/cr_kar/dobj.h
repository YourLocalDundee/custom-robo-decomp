/* Custom Robo override of doldecomp/kar src/sysdolphin/dobj.h (step 61): found before the kar copy in
 * include/kar/sysdolphin (decomp/configure.py CR_OBJECT_INCLUDES), so kar's dobj.c compiles unchanged. Names only:
 * kar's dobj.c calls _OSReport (DObjLoad) and HSD_PObjRemoveAnimAll (HSD_DObjAddAnim); the function at those call
 * sites is OSReport (0x801347F4) and HSD_PObjAddAnimAll (0x80193968) in symbols/symbols.txt, the names the wowjinxy
 * KAR source and Melee's dobj.c use there. The header itself is the kar copy, unchanged. */
#define _OSReport OSReport
#define HSD_PObjRemoveAnimAll HSD_PObjAddAnimAll
#include "../kar/sysdolphin/dobj.h"
