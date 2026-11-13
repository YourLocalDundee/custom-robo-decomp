/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_801155AC: .text 0x801155AC-0x80115918 (876 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008E8F4(u32 arg0, u32 arg1);
void fn_8008E7EC(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern const f32 lbl_80451920;
extern const f32 lbl_80451924;
void fn_8008E5D0(void *arg0);
void GXClearVtxDesc(void);
void GXSetNumTexGens(u8 nTexGens);
typedef enum _GXAttr
{
  GX_VA_PNMTXIDX,
  GX_VA_TEX0MTXIDX,
  GX_VA_TEX1MTXIDX,
  GX_VA_TEX2MTXIDX,
  GX_VA_TEX3MTXIDX,
  GX_VA_TEX4MTXIDX,
  GX_VA_TEX5MTXIDX,
  GX_VA_TEX6MTXIDX,
  GX_VA_TEX7MTXIDX,
  GX_VA_POS,
  GX_VA_NRM,
  GX_VA_CLR0,
  GX_VA_CLR1,
  GX_VA_TEX0,
  GX_VA_TEX1,
  GX_VA_TEX2,
  GX_VA_TEX3,
  GX_VA_TEX4,
  GX_VA_TEX5,
  GX_VA_TEX6,
  GX_VA_TEX7,
  GX_POS_MTX_ARRAY,
  GX_NRM_MTX_ARRAY,
  GX_TEX_MTX_ARRAY,
  GX_LIGHT_ARRAY,
  GX_VA_NBT,
  GX_VA_MAX_ATTR,
  GX_VA_NULL = 0xFF
} GXAttr;
typedef enum _GXAttrType
{
  GX_NONE,
  GX_DIRECT,
  GX_INDEX8,
  GX_INDEX16
} GXAttrType;
void GXSetVtxDesc(GXAttr attr, GXAttrType type);
typedef enum _GXVtxFmt
{
  GX_VTXFMT0,
  GX_VTXFMT1,
  GX_VTXFMT2,
  GX_VTXFMT3,
  GX_VTXFMT4,
  GX_VTXFMT5,
  GX_VTXFMT6,
  GX_VTXFMT7,
  GX_MAX_VTXFMT
} GXVtxFmt;
typedef enum _GXCompCnt
{
  GX_POS_XY = 0,
  GX_POS_XYZ = 1,
  GX_NRM_XYZ = 0,
  GX_NRM_NBT = 1,
  GX_NRM_NBT3 = 2,
  GX_CLR_RGB = 0,
  GX_CLR_RGBA = 1,
  GX_TEX_S = 0,
  GX_TEX_ST = 1
} GXCompCnt;
typedef enum _GXCompType
{
  GX_U8 = 0,
  GX_S8 = 1,
  GX_U16 = 2,
  GX_S16 = 3,
  GX_F32 = 4,
  GX_RGB565 = 0,
  GX_RGB8 = 1,
  GX_RGBX8 = 2,
  GX_RGBA4 = 3,
  GX_RGBA6 = 4,
  GX_RGBA8 = 5
} GXCompType;
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac);
void GXSetNumChans(u8 nChans);
void GXSetNumTevStages(u8 nStages);
typedef enum _GXChannelID
{
  GX_COLOR0,
  GX_COLOR1,
  GX_ALPHA0,
  GX_ALPHA1,
  GX_COLOR0A0,
  GX_COLOR1A1,
  GX_COLOR_ZERO,
  GX_ALPHA_BUMP,
  GX_ALPHA_BUMPN,
  GX_COLOR_NULL = 0xFF
} GXChannelID;
typedef u8 GXBool;
typedef enum _GXColorSrc
{
  GX_SRC_REG,
  GX_SRC_VTX
} GXColorSrc;
typedef enum _GXDiffuseFn
{
  GX_DF_NONE,
  GX_DF_SIGN,
  GX_DF_CLAMP
} GXDiffuseFn;
typedef enum _GXAttnFn
{
  GX_AF_SPEC,
  GX_AF_SPOT,
  GX_AF_NONE
} GXAttnFn;
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn);
typedef enum _GXBlendMode
{
  GX_BM_NONE,
  GX_BM_BLEND,
  GX_BM_LOGIC,
  GX_BM_SUBTRACT,
  GX_MAX_BLENDMODE
} GXBlendMode;
typedef enum _GXBlendFactor
{
  GX_BL_ZERO,
  GX_BL_ONE,
  GX_BL_SRCCLR,
  GX_BL_INVSRCCLR,
  GX_BL_SRCALPHA,
  GX_BL_INVSRCALPHA,
  GX_BL_DSTALPHA,
  GX_BL_INVDSTALPHA,
  GX_BL_DSTCLR = GX_BL_SRCCLR,
  GX_BL_INVDSTCLR = GX_BL_INVSRCCLR
} GXBlendFactor;
typedef enum _GXLogicOp
{
  GX_LO_CLEAR,
  GX_LO_AND,
  GX_LO_REVAND,
  GX_LO_COPY,
  GX_LO_INVAND,
  GX_LO_NOOP,
  GX_LO_XOR,
  GX_LO_OR,
  GX_LO_NOR,
  GX_LO_EQUIV,
  GX_LO_INV,
  GX_LO_REVOR,
  GX_LO_INVCOPY,
  GX_LO_INVOR,
  GX_LO_NAND,
  GX_LO_SET
} GXLogicOp;
void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op);
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
void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable);
typedef enum _GXTevStageID
{
  GX_TEVSTAGE0,
  GX_TEVSTAGE1,
  GX_TEVSTAGE2,
  GX_TEVSTAGE3,
  GX_TEVSTAGE4,
  GX_TEVSTAGE5,
  GX_TEVSTAGE6,
  GX_TEVSTAGE7,
  GX_TEVSTAGE8,
  GX_TEVSTAGE9,
  GX_TEVSTAGE10,
  GX_TEVSTAGE11,
  GX_TEVSTAGE12,
  GX_TEVSTAGE13,
  GX_TEVSTAGE14,
  GX_TEVSTAGE15,
  GX_MAX_TEVSTAGE
} GXTevStageID;
typedef enum _GXTexCoordID
{
  GX_TEXCOORD0,
  GX_TEXCOORD1,
  GX_TEXCOORD2,
  GX_TEXCOORD3,
  GX_TEXCOORD4,
  GX_TEXCOORD5,
  GX_TEXCOORD6,
  GX_TEXCOORD7,
  GX_MAX_TEXCOORD,
  GX_TEXCOORD_NULL = 0xFF
} GXTexCoordID;
typedef enum _GXTexMapID
{
  GX_TEXMAP0,
  GX_TEXMAP1,
  GX_TEXMAP2,
  GX_TEXMAP3,
  GX_TEXMAP4,
  GX_TEXMAP5,
  GX_TEXMAP6,
  GX_TEXMAP7,
  GX_MAX_TEXMAP,
  GX_TEXMAP_NULL = 0xFF,
  GX_TEX_DISABLE = 0x100
} GXTexMapID;
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color);
typedef enum _GXTevColorArg
{
  GX_CC_CPREV,
  GX_CC_APREV,
  GX_CC_C0,
  GX_CC_A0,
  GX_CC_C1,
  GX_CC_A1,
  GX_CC_C2,
  GX_CC_A2,
  GX_CC_TEXC,
  GX_CC_TEXA,
  GX_CC_RASC,
  GX_CC_RASA,
  GX_CC_ONE,
  GX_CC_HALF,
  GX_CC_KONST,
  GX_CC_ZERO,
  GX_CC_TEXRRR,
  GX_CC_TEXGGG,
  GX_CC_TEXBBB,
  GX_CC_QUARTER = GX_CC_KONST
} GXTevColorArg;
void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d);
typedef enum _GXTevAlphaArg
{
  GX_CA_APREV,
  GX_CA_A0,
  GX_CA_A1,
  GX_CA_A2,
  GX_CA_TEXA,
  GX_CA_RASA,
  GX_CA_KONST,
  GX_CA_ZERO,
  GX_CA_ONE = GX_CA_KONST
} GXTevAlphaArg;
void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d);
typedef enum _GXTevOp
{
  GX_TEV_ADD = 0,
  GX_TEV_SUB = 1,
  GX_TEV_COMP_R8_GT = 8,
  GX_TEV_COMP_R8_EQ = 9,
  GX_TEV_COMP_GR16_GT = 10,
  GX_TEV_COMP_GR16_EQ = 11,
  GX_TEV_COMP_BGR24_GT = 12,
  GX_TEV_COMP_BGR24_EQ = 13,
  GX_TEV_COMP_RGB8_GT = 14,
  GX_TEV_COMP_RGB8_EQ = 15,
  GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT,
  GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ
} GXTevOp;
typedef enum _GXTevBias
{
  GX_TB_ZERO,
  GX_TB_ADDHALF,
  GX_TB_SUBHALF,
  GX_MAX_TEVBIAS
} GXTevBias;
typedef enum _GXTevScale
{
  GX_CS_SCALE_1,
  GX_CS_SCALE_2,
  GX_CS_SCALE_4,
  GX_CS_DIVIDE_2,
  GX_MAX_TEVSCALE
} GXTevScale;
typedef enum _GXTevRegID
{
  GX_TEVPREV,
  GX_TEVREG0,
  GX_TEVREG1,
  GX_TEVREG2,
  GX_MAX_TEVREG
} GXTevRegID;
void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
typedef enum _GXPrimitive
{
  GX_QUADS = 0x80,
  GX_TRIANGLES = 0x90,
  GX_TRIANGLESTRIP = 0x98,
  GX_TRIANGLEFAN = 0xA0,
  GX_LINES = 0xA8,
  GX_LINESTRIP = 0xB0,
  GX_POINTS = 0xB8
} GXPrimitive;
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);
extern u8 lbl_8041C408[1056];
extern u8 lbl_8041C0C0[840];

/* fn_801155AC: Nov 6, 2026 hand draft (m2c 85%): the GX pipe colour writes are bytes, the second record address is computed straight into its local before the two position writes */

void fn_801155AC(void) {
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    s16 var_r31;
    void *var_r30;
    u8 *var_r29;

    fn_8008E8F4(0x1000U, 0x800U);
    fn_8008E7EC(0U, 0U, 0x280U, 0x1E0U);
    sp8 = lbl_80451920;
    spC = lbl_80451920;
    sp10 = lbl_80451920;
    sp14 = lbl_80451924;
    fn_8008E5D0(&sp8);
    GXClearVtxDesc();
    GXSetNumTexGens(0U);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_S, GX_RGBA6, 0U);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
    GXSetNumChans(1U);
    GXSetNumTevStages(1U);
    GXSetChanCtrl(GX_COLOR0A0, 0U, GX_SRC_VTX, GX_SRC_VTX, 0U, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(1U, GX_LEQUAL, 1U);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
    GXBegin(GX_LINES, GX_VTXFMT0, (M2C_FIELD(lbl_8041C408, u16 *, 0x44) * 2) & 0xFFFE);
    var_r31 = 0;
    while (var_r31 < (s32) M2C_FIELD(lbl_8041C408, u16 *, 0x44)) {
        var_r30 = lbl_8041C0C0 + (var_r31 * 0x1C);
        var_f30 = M2C_FIELD(var_r30, f32 *, 4);
        var_f31 = M2C_FIELD(var_r30, f32 *, 0);
        var_r29 = lbl_8041C0C0 + (var_r31 * 0x1C) + 0xC;
        *(f32 *)0xCC008000 = var_f31;
        *(f32 *)0xCC008000 = var_f30;
        if (var_r31 < 2) {
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
        } else {
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0xFF;
        }
        var_f28 = M2C_FIELD(var_r29, f32 *, 4);
        var_f29 = M2C_FIELD(var_r29, f32 *, 0);
        *(f32 *)0xCC008000 = var_f29;
        *(f32 *)0xCC008000 = var_f28;
        if (var_r31 < 2) {
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
        } else {
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0xFF;
        }
        var_r31 += 1;
    }
    M2C_FIELD(lbl_8041C408, u16 *, 0x44) = 0U;
}
