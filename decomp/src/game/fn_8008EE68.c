/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008EE68: .text 0x8008EE68-0x8008EFC8 (352 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct _GXTexObj
{
  u32 dummy[8];
} GXTexObj;
typedef enum _GXTexFmt
{
  GX_TF_I4 = 0x0,
  GX_TF_I8 = 0x1,
  GX_TF_IA4 = 0x2,
  GX_TF_IA8 = 0x3,
  GX_TF_RGB565 = 0x4,
  GX_TF_RGB5A3 = 0x5,
  GX_TF_RGBA8 = 0x6,
  GX_TF_CMPR = 0xE,
  GX_CTF_R4 = 0x0 | 0x20,
  GX_CTF_RA4 = 0x2 | 0x20,
  GX_CTF_RA8 = 0x3 | 0x20,
  GX_CTF_YUVA8 = 0x6 | 0x20,
  GX_CTF_A8 = 0x7 | 0x20,
  GX_CTF_R8 = 0x8 | 0x20,
  GX_CTF_G8 = 0x9 | 0x20,
  GX_CTF_B8 = 0xA | 0x20,
  GX_CTF_RG8 = 0xB | 0x20,
  GX_CTF_GB8 = 0xC | 0x20,
  GX_TF_Z8 = 0x1 | 0x10,
  GX_TF_Z16 = 0x3 | 0x10,
  GX_TF_Z24X8 = 0x6 | 0x10,
  GX_CTF_Z4 = (0x0 | 0x10) | 0x20,
  GX_CTF_Z8M = (0x9 | 0x10) | 0x20,
  GX_CTF_Z8L = (0xA | 0x10) | 0x20,
  GX_CTF_Z16L = (0xC | 0x10) | 0x20,
  GX_TF_A8 = GX_CTF_A8,
  GX_TF_C4 = 0x8,
  GX_TF_C8 = 0x9,
  GX_TF_C14X2 = 0xA
} GXTexFmt;
typedef enum _GXTexWrapMode
{
  GX_CLAMP,
  GX_REPEAT,
  GX_MIRROR,
  GX_MAX_TEXWRAPMODE
} GXTexWrapMode;
void GXInitTexObjCI(GXTexObj *obj, void *image_ptr, u16 width, u16 height, GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap, u32 tlut_name);
typedef enum _GXTexFilter
{
  GX_NEAR,
  GX_LINEAR,
  GX_NEAR_MIP_NEAR,
  GX_LIN_MIP_NEAR,
  GX_NEAR_MIP_LIN,
  GX_LIN_MIP_LIN
} GXTexFilter;
typedef u8 GXBool;
typedef enum _GXAnisotropy
{
  GX_ANISO_1,
  GX_ANISO_2,
  GX_ANISO_4,
  GX_MAX_ANISOTROPY
} GXAnisotropy;
void GXInitTexObjLOD(GXTexObj *obj, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp, GXBool do_edge_lod, GXAnisotropy max_aniso);
extern const f32 lbl_80451008;
extern u8 lbl_8038B430[24576];

/* fn_8008EE68: Nov 13, 2026 hand draft: texture-object cache: GXInitTexObjCI with its ninth (TLUT) argument, 8-word entries as u32 (*)[8] */

extern u8 *lbl_8044F694;

void fn_8008EE68(u8 *arg0, void *arg1) {
    GXTexObj sp10;
    u16 var_r31;
    u16 var_r30;
    u16 var_r29;

    GXInitTexObjCI(&sp10, arg0 + 0x20, M2C_FIELD(arg0, u16 *, 0x1A), M2C_FIELD(arg0, u16 *, 0x1C), GX_TF_I4, GX_CLAMP, GX_CLAMP, 0U, 1);
    GXInitTexObjLOD(&sp10, GX_LINEAR, GX_LINEAR, lbl_80451008, lbl_80451008, lbl_80451008, 0U, 0U, GX_ANISO_1);
    var_r30 = 0;
    lbl_8044F694 = lbl_8038B430;
    for (; !(var_r30 >= 0x100U); var_r30 += 1) {
        for (var_r31 = 0; ((u32 *) &sp10)[var_r31] == ((u32 (*)[8]) lbl_8044F694)[var_r30][var_r31]; var_r31 += 1) {
            if (var_r31 >= 7U) {
                M2C_FIELD(arg1, u16 *, 0x10) = var_r30;
                return;
            }
        }
        for (var_r31 = 0; ((u32 (*)[8]) lbl_8044F694)[var_r30][var_r31] == 0U; var_r31 += 1) {
            if (var_r31 >= 7U) {
                M2C_FIELD(arg1, u16 *, 0x10) = var_r30;
                var_r29 = 0;
                while (var_r29 < 8U) {
                    ((u32 (*)[8]) lbl_8044F694)[var_r30][var_r29] = ((u32 *) &sp10)[var_r29];
                    var_r29 += 1;
                }
                return;
            }
        }
    }
    return;
}
