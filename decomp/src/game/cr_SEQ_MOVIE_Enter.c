/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_MOVIE_Enter: .text 0x80016824-0x80016868 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void HSD_StateInvalidate(int mask);
typedef enum _GXCompare
{
  GX_NEVER,
  GX_LESS,
  GX_EQUAL,
  GX_LEQUAL,
  GX_GREATER,
  GX_NEQUAL,
  GX_GEQUAL,
  GX_ALWAYS
} GXCompare;
typedef enum _GXAlphaOp
{
  GX_AOP_AND,
  GX_AOP_OR,
  GX_AOP_XOR,
  GX_AOP_XNOR,
  GX_MAX_ALPHAOP
} GXAlphaOp;
void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1);
void fn_80096F54(void);
void fn_80097530(void);

void cr_SEQ_MOVIE_Enter(void) {
    HSD_StateInvalidate(-1);
    GXSetAlphaCompare(GX_ALWAYS, 0U, GX_AOP_OR, GX_ALWAYS, 0x50U);
    fn_80096F54();
    fn_80097530();
}
