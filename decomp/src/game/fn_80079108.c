/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80079108: .text 0x80079108-0x80079214 (268 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
void fn_8018CCDC(void *arg0);
void fn_80078B60(void);
extern u32 lbl_8044F608[2];
extern u32 lbl_8037A400[12032];
void fn_8017EE20(void *arg0);
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjAnimAll(HSD_JObj *);
void fn_8018CD2C(u32 arg0);

void fn_80079108(void) {
    u32 temp_r31_27;
    s32 var_r30_18;
    u32 var_r29_34;
    u32 var_r28_33;

    if (!(M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x38) & 0x4000)) {
        fn_8018CCDC(fn_80078B60);
        var_r30_18 = 0;
        *lbl_8044F608 = (u32) lbl_8037A400;
        while (var_r30_18 < 0x100) {
            if (M2C_FIELD(*lbl_8044F608, u32 *, 8) != 0U) {
                temp_r31_27 = *lbl_8044F608;
                if (M2C_FIELD(temp_r31_27, u32 *, 0) != 0U) {
                    fn_8017EE20(M2C_FIELD(temp_r31_27, HSD_JObj **, 0x90));
                    var_r28_33 = temp_r31_27;
                    var_r29_34 = 0U;
                    while (var_r29_34 < 0xBCU) {
                        var_r29_34 += 1;
                        M2C_FIELD(var_r28_33, s8 *, 0) = 0;
                        var_r28_33 += 1;
                    }
                }
            }
            if (M2C_FIELD(*lbl_8044F608, u32 *, 0) != 0U) {
                HSD_JObjAnimAll(M2C_FIELD(*lbl_8044F608, HSD_JObj **, 0x90));
                M2C_FIELD(*lbl_8044F608, s32 *, 0xC) += 1;
            }
            var_r30_18 += 1;
            *lbl_8044F608 += 0xBC;
        }
        fn_8018CD2C((u32) fn_80078B60);
    }
}
