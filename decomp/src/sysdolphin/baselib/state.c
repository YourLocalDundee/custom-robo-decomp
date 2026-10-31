/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/state.c */ #include "state.h"

#include "lobj.h"
#include "mobj.h"
#include "pobj.h"
#include "tev.h"
#include "texp.h"
#include "util.h"
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXPixel.h>
#include <dolphin/gx/GXTev.h>

static GXColor dark_matter = { 0x00, 0x00, 0x00, 0xFF };

/* Custom Robo: this revision has no HSD_StateSetLineWidth / HSD_StateSetPointSize (main.dol: their statics too),
   point size before line width (main.dol's .sbss: line width +0x38, point size +0x39) */
static u8 state_point_size;
static u8 state_line_width;
static int state_cull_mode;
static int state_blend_type;
static int state_src_factor;
static int state_dst_factor;
static int state_logic_op;
static u8 state_z_enable;
static int state_z_func;
static u8 state_z_update;
static int state_alpha_comp0;
static u8 state_alpha_ref0;
static int state_alpha_op;
static int state_alpha_comp1;
static u8 state_alpha_ref1;
static u8 state_color_update;
static u8 state_alpha_update;
static u8 state_enable_dst_alpha;
static u8 state_dst_alpha;
static u8 state_before_tex;
static u8 state_dither;

static struct matstate_t {
    GXColor ambient;
    GXColor diffuse;
    GXColor specular;
    u8 alpha;
    f32 shininess;
} matstate;

/* Custom Robo: this revision's six channel templates (main.dol's .data 0x8024BAE8-0x8024BC08; Melee has six global
   structs of other values): file statics, which the compiler addresses from one base register as main.dol does;
   specular, diffuse, alpha, alpha as color0a0, vertex color, vertex alpha (named by address) */
static HSD_Chan lbl_8024BAE8 = { NULL, 1, 0, { 0 }, { 0xFF, 0xFF, 0xFF, 0xFF }, 1, 0, 0, 0, 0, 0 };
static HSD_Chan lbl_8024BB18 = { NULL, 0, 0, { 0 }, { 0xFF, 0xFF, 0xFF, 0xFF }, 1, 0, 0, 0, 2, 1 };
static HSD_Chan lbl_8024BB48 = { NULL, 2, 0, { 0 }, { 0 }, 1, 0, 0, 0, 2, 1 };
static HSD_Chan lbl_8024BB78 = { NULL, 2, 0, { 0 }, { 0 }, 0, 0, 1, 0, 2, 1 };
static HSD_Chan lbl_8024BBA8 = { NULL, 0, 0, { 0 }, { 0xFF, 0xFF, 0xFF, 0xFF }, 0, 0, 1, 0, 0, 2 };
static HSD_Chan lbl_8024BBD8 = { NULL, 2, 0, { 0 }, { 0x00, 0x00, 0x00, 0xFF }, 0, 0, 1, 0, 0, 2 };

struct invalidate_funcs_t {
    int mask;
    void (*func)(void);
} invalidate_funcs[] = {
    { HSD_STATE_PRIMITIVE, _HSD_StateInvalidatePrimitive },
    { HSD_STATE_VTX_ATTR, _HSD_StateInvalidateVtxAttr },
    { HSD_STATE_COLOR_CHANNEL, _HSD_StateInvalidateColorChannel },
    { HSD_STATE_TEV_STAGE, _HSD_StateInvalidateTevStage },
    { HSD_STATE_TEV_REGISTER, _HSD_StateInvalidateTevRegister },
    { HSD_STATE_TEX_COORD_GEN, _HSD_StateInvalidateTexCoordGen },
    { HSD_STATE_RENDER_MODE, _HSD_StateInvalidateRenderMode },
    { HSD_STATE_NONE }
};

/* Custom Robo: this revision's HSD_SetupChannelMode (= doldecomp/kar's tev.s, KAR PAL), written to it after
   wowjinxy/KAR tev.c's (cb94b15) with Melee's field names */
void HSD_DisableChannelLighting(int mask);

void HSD_SetupChannelMode(u32 rendermode)
{
    HSD_LObj* lobj;
    HSD_LObj* diffuse_lobj;
    int i;
    int max;
    u32 num_chans;
    u32 alpha_mode;
    int specular = 0;
    int alpha_color = 0;
    u8 alpha;

    num_chans = rendermode & 3;
    if (num_chans == 0) {
        num_chans = 1;
    }
    alpha_mode = rendermode & 0x6000;
    if (alpha_mode == 0) {
        alpha_mode = num_chans << 13;
    }

    if (rendermode & 8) {
        lbl_8024BAE8.light_mask = HSD_LObjGetLightMaskSpecular();
        HSD_SetupChannel(&lbl_8024BAE8);
        specular = 1;
        max = HSD_LObjGetNbActive();
        for (i = 0; i < max; i++) {
            lobj = HSD_LObjGetActiveByIndex(i);
            if (lobj != NULL) {
                HSD_LObjSetup(lobj, lobj->color, matstate.shininess);
            }
        }
    }

    if (rendermode & 4) {
        diffuse_lobj = HSD_LObjGetActiveByID(0x100);
        if (diffuse_lobj != NULL && (diffuse_lobj->flags & 4)) {
            HSD_MulColor(&matstate.ambient, &diffuse_lobj->color, &lbl_8024BB18.amb_color);
        } else {
            lbl_8024BB18.amb_color = dark_matter;
        }
        lbl_8024BB18.mat_src = (num_chans >> 1) & 1;
        lbl_8024BB18.light_mask = HSD_LObjGetLightMaskDiffuse();
        HSD_SetupChannel(&lbl_8024BB18);

        if (alpha_mode & 0x4000) {
            lbl_8024BB48.chan = 3;
            alpha_color = 1;
            HSD_SetupChannel(&lbl_8024BB78);
        } else {
            lbl_8024BB48.chan = 2;
        }

        lbl_8024BB48.light_mask = HSD_LObjGetLightMaskAlpha();
        if (diffuse_lobj != NULL && (diffuse_lobj->flags & 0x10)) {
            alpha = diffuse_lobj->color.a;
        } else {
            alpha = 0;
        }
        if (lbl_8024BB48.light_mask != 0) {
            lbl_8024BB48.enable = 1;
            lbl_8024BB48.mat_color.a = 0xFF;
            lbl_8024BB48.amb_color.a = alpha;
        } else {
            lbl_8024BB48.enable = 0;
            lbl_8024BB48.mat_color.a = alpha;
        }
        HSD_SetupChannel(&lbl_8024BB48);
    } else {
        lbl_8024BBA8.mat_src = (num_chans >> 1) & 1;
        HSD_SetupChannel(&lbl_8024BBA8);
        lbl_8024BBD8.mat_src = (alpha_mode >> 14) & 1;
        HSD_SetupChannel(&lbl_8024BBD8);
    }

    if (specular) {
        if (!alpha_color) {
            HSD_DisableChannelLighting(3);
        }
        HSD_StateSetNumChans(2);
    } else if (alpha_color) {
        HSD_DisableChannelLighting(1);
        HSD_StateSetNumChans(2);
    } else {
        HSD_DisableChannelLighting(5);
        HSD_StateSetNumChans(1);
    }
}

void HSD_SetupPEMode(u32 flags, HSD_PEDesc* pe)
{
    u8 var_r3_2;
    int var_r3;

    if (pe != NULL) {
        HSD_StateSetColorUpdate(pe->flags & 1);
        HSD_StateSetAlphaUpdate(pe->flags & 2);
        HSD_StateSetDstAlpha(pe->flags & 4, pe->dst_alpha);
        HSD_StateSetBlendMode(pe->type, pe->src_factor, pe->dst_factor,
                              pe->logic_op);
        HSD_StateSetZMode(pe->flags & 0x10, pe->z_comp, pe->flags & 0x20);
        HSD_StateSetZCompLoc(pe->flags & 8);
        HSD_StateSetAlphaCompare(pe->alpha_comp0, pe->ref0, pe->alpha_op,
                                 pe->alpha_comp1, pe->ref1);
        HSD_StateSetDither(pe->flags & 0x40);
        return;
    }
    HSD_StateSetColorUpdate(1);
    HSD_StateSetAlphaUpdate(0);
    HSD_StateSetDstAlpha(0, 0U);
    if (flags & 0x40000000) {
        var_r3 = 1;
    } else {
        var_r3 = 0;
    }
    HSD_StateSetBlendMode(var_r3, 4U, 5U, 0xFU);
    var_r3_2 = (flags & 0x20000000) ? GX_FALSE : GX_TRUE;
    HSD_StateSetZMode(1, (flags & 0x08000000) ? 7 : 3, var_r3_2);
    if (!(flags & 0x20000000) && (flags & 0x40000000)) {
        HSD_StateSetZCompLoc(0);
        HSD_StateSetAlphaCompare(4U, 0U, 0U, 4U, 0U);
    } else {
        HSD_StateSetZCompLoc(1);
        HSD_StateSetAlphaCompare(7U, 0U, 0U, 7U, 0U);
    }
    HSD_StateSetDither(0);
}

static void setupTevMode_last(void)
{
    if (!HSD_StateGetNumTevStages()) {
        HSD_TevDesc tevdesc;
        tevdesc.flags = 0;
        tevdesc.stage = HSD_StateAssignTev();
        tevdesc.coord = 0xFF;
        tevdesc.map = GX_TEXMAP_NULL;
        tevdesc.color = GX_COLOR0A0;
        tevdesc.u.tevop.tevmode = GX_PASSCLR;
        HSD_SetupTevStage(&tevdesc);
    }
}

void HSD_SetupRenderModeWithCustomPE(u32 rendermode, HSD_PEDesc* pe)
{
    setupTevMode_last();
    HSD_SetupPEMode(rendermode, pe);
    HSD_SetTevRegAll();
    HSD_StateSetNumTevStages();
    HSD_StateSetNumTexGens();
    HSD_SetupChannelMode(rendermode);
}

void HSD_SetupRenderMode(u32 rendermode)
{
    HSD_SetupRenderModeWithCustomPE(rendermode, NULL);
}

/* Custom Robo: the alpha clamp is an inline function with early returns in this revision (main.dol keeps the
 * result in f2 and loads 255.0F after it); the name is ours, an inline leaves no symbol */
static inline f32 cr_clamp01(f32 x)
{
    if (x <= 0.0F) {
        return 0.0F;
    }
    if (x >= 1.0F) {
        return 1.0F;
    }
    return x;
}

void HSD_SetMaterialColor(GXColor ambient, GXColor diffuse, GXColor specular,
                          f32 alpha)
{
    matstate.ambient = ambient;
    matstate.diffuse = diffuse;
    matstate.specular = specular;
    matstate.alpha = 255.0F * cr_clamp01(alpha);
}

void HSD_SetMaterialShininess(f32 shininess)
{
    matstate.shininess = shininess;
}

#if 0 /* Custom Robo: not in main.dol */
void HSD_StateSetLineWidth(u8 arg0, int arg1)
{
    if (state_line_width != arg0 || state_line_tex_offsets != arg1) {
        GXSetLineWidth(arg0, arg1);
        state_line_width = arg0;
        state_line_tex_offsets = arg1;
    }
}
#endif

void HSD_StateSetCullMode(int mode)
{
    if (state_cull_mode != mode) {
        GXSetCullMode(mode);
        state_cull_mode = mode;
    }
}

void HSD_StateSetBlendMode(int type, int src_factor, int dst_factor, int op)
{
    if (state_blend_type != type || state_src_factor != src_factor ||
        state_dst_factor != dst_factor || state_logic_op != op)
    {
        GXSetBlendMode(type, src_factor, dst_factor, op);
        state_blend_type = type;
        state_src_factor = src_factor;
        state_dst_factor = dst_factor;
        state_logic_op = op;
    }
}

void HSD_StateSetZMode(u32 arg0, int arg1, u32 arg2) /* Custom Robo: u32 (cmplw in main.dol) */
{
    /* Custom Robo: arg2 first (main.dol: arg0 in r31, arg2 in r30) */
    arg2 = arg2 ? GX_TRUE : GX_FALSE;
    arg0 = arg0 ? GX_TRUE : GX_FALSE;

    if (state_z_enable != arg0 || state_z_func != arg1 ||
        state_z_update != arg2)
    {
        GXSetZMode(arg0, arg1, arg2);
        state_z_enable = arg0;
        state_z_func = arg1;
        state_z_update = arg2;
    }
}

#if 0 /* Custom Robo: not in main.dol */
void HSD_StateSetPointSize(u8 arg0, int arg1)
{
    if (state_point_size != arg0 || HSD_State_804D75BC != arg1) {
        GXSetPointSize(arg0, arg1);
        state_point_size = arg0;
        HSD_State_804D75BC = arg1;
    }
}
#endif

void HSD_StateSetAlphaCompare(int arg0, u8 arg1, int arg2, int arg3, u8 arg4)
{
    if (state_alpha_comp0 != arg0 || state_alpha_ref0 != arg1 ||
        state_alpha_op != arg2 || state_alpha_comp1 != arg3 ||
        state_alpha_ref1 != arg4)
    {
        GXSetAlphaCompare(arg0, arg1, arg2, arg3, arg4);
        state_alpha_comp0 = arg0;
        state_alpha_ref0 = arg1;
        state_alpha_op = arg2;
        state_alpha_comp1 = arg3;
        state_alpha_ref1 = arg4;
    }
}

void HSD_StateSetColorUpdate(u32 arg0) /* Custom Robo: u32 (main.dol compares unsigned, cmplw) */
{
    arg0 = arg0 ? GX_TRUE : GX_FALSE;
    if (state_color_update != arg0) {
        GXSetColorUpdate(arg0);
        state_color_update = arg0;
    }
}

void HSD_StateSetAlphaUpdate(u32 arg0) /* Custom Robo: u32 (main.dol compares unsigned, cmplw) */
{
    arg0 = arg0 ? GX_TRUE : GX_FALSE;
    if (state_alpha_update != arg0) {
        GXSetAlphaUpdate(arg0);
        state_alpha_update = arg0;
    }
}

void HSD_StateSetDstAlpha(u32 arg0, u8 arg1) /* Custom Robo: u32 (main.dol compares unsigned, cmplw) */
{
    arg0 = arg0 ? GX_TRUE : GX_FALSE;
    if (state_enable_dst_alpha != arg0 || state_dst_alpha != arg1) {
        GXSetDstAlpha(arg0, arg1);
        state_enable_dst_alpha = arg0;
        state_dst_alpha = arg1;
    }
}

void HSD_StateSetZCompLoc(u32 arg0) /* Custom Robo: u32 (main.dol compares unsigned, cmplw) */
{
    arg0 = arg0 ? GX_TRUE : GX_FALSE;
    if (state_before_tex != arg0) {
        GXSetZCompLoc(arg0);
        state_before_tex = (u8) arg0;
    }
}

void HSD_StateSetDither(u32 arg0) /* Custom Robo: u32 (main.dol compares unsigned, cmplw) */
{
    arg0 = arg0 ? GX_TRUE : GX_FALSE;
    if (state_dither != arg0) {
        GXSetDither(arg0);
        state_dither = arg0;
    }
}

void _HSD_StateInvalidatePrimitive(void)
{
    state_line_width = 0;
    state_cull_mode = -1; /* Custom Robo: this order and 0xFF (main.dol) */
    state_point_size = 0xFF;
}

void _HSD_StateInvalidateVtxAttr(void)
{
    HSD_ClearVtxDesc();
}

void _HSD_StateInvalidateRenderMode(void)
{
    state_blend_type = -1;
    state_src_factor = -1;
    state_dst_factor = -1;
    state_logic_op = -1;
    state_z_enable = -1;
    state_z_func = -1;
    state_z_update = -1;
    state_alpha_comp0 = -1;
    state_alpha_ref0 = 0;
    state_alpha_op = -1;
    state_alpha_comp1 = -1;
    state_alpha_ref1 = 0;
    state_color_update = -1;
    state_alpha_update = -1;
    state_enable_dst_alpha = -1;
    state_dst_alpha = 0;
    state_before_tex = -1;
    state_dither = -1;
}

void HSD_StateInvalidate(int mask)
{
    int i;
    for (i = 0; invalidate_funcs[i].mask != HSD_STATE_NONE; i++) {
        if (mask & invalidate_funcs[i].mask) {
            invalidate_funcs[i].func();
        }
    }
}
