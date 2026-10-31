/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/bytecode.c */ #include "bytecode.h"

#include <Runtime/platform.h>

#include <math.h>

#include "debug.h"
#include "list.h"
#include "random.h"
#include "util.h"
#include <dolphin/os.h>

/* Custom Robo: this revision calls MSL's double functions and rounds the result (main.dol: bl sin / cos / tan / asin /
 * acos / atan / log / exp / fmod / pow / atan2, then frsp), where Melee's calls the float versions */
double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan(double);
double log(double);
double exp(double);
double fmod(double, double);
double pow(double, double);
double atan2(double, double);

typedef union {
    void* p;
    int i;
    f32 f;
} ByteCodeVal;

float HSD_ByteCodeEval(u8* bytecode, const f32* args, s32 nb_args)
{
    HSD_SList* stack;
    int i;
    u8 last_command;
    s32 operand_count;
    u32 operand;
    HSD_SList* list;
    f32 fv, f0, f1;
    s32 d0, d1;

    stack = NULL;
    operand_count = 0;

    if (bytecode == NULL) {
        return 0.0F;
    }

    for (;;) {
        if (operand_count > 0) {
            operand_count--;
            operand = (operand << 8) | (*bytecode & 0xFF);
            bytecode++;

            if (operand_count != 0) {
                continue;
            }

            switch (last_command) {
            case 2:
                HSD_ASSERT(281, operand < nb_args);
                stack = HSD_SListAllocAndPrepend(
                    stack, (void*) ((ByteCodeVal*) &args[operand])->i);
                break;
            case 5:
                for (i = 0; i < operand; i++) {
                    stack = HSD_SListRemove(stack);
                }
                break;
            case 0x3C: {
                list = stack;
                i = 0;
                while (list != NULL && i < operand) {
                    list = list->next;
                    i++;
                }
                if (list == NULL) {
                    OSReport("specified stack doesn't exist (%d).\n", operand);
                    HSD_Panic(__FILE__, 299, "");
                } else {
                    stack = HSD_SListAllocAndPrepend(stack, list->data);
                }
                break;
            }
            case 3:
                HSD_ASSERT(307, stack);
                if ((int) stack->data != 0) {
                    bytecode += operand;
                }
                stack = HSD_SListRemove(stack);
                break;
            case 4:
                bytecode += operand;
                break;
            case 6:
                stack = HSD_SListAllocAndPrepend(stack, (void*) operand);
                break;
            case 0xFF:
                HSD_Panic(__FILE__, 323, "not yet implemented.\n");
                /* fallthrough */
            default:
                HSD_Panic(__FILE__, 326, "unexpected byte code.\n");
                break;
            }
            continue;
        }

        last_command = *bytecode++;
        switch (last_command) {
        case 0:
            break;
        case 1:
            HSD_ASSERT(339, stack);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            while (stack != NULL) {
                stack = HSD_SListRemove(stack);
            }
            return f0;
        case 5:
        case 0x3C:
        case 0xFF:
            operand_count = 1;
            operand = 0;
            break;
        case 2:
        case 3:
        case 4:
            operand_count = 2;
            operand = 0;
            break;
        case 6:
            operand_count = 4;
            operand = 0;
            break;
        case 7:
            HSD_ASSERT(376, stack);
            {
                ((ByteCodeVal*) &stack->data)->i =
                    (int) ((ByteCodeVal*) &stack->data)->f;
            }
            break;
        case 8:
            HSD_ASSERT(381, stack);
            {
                fv = (f32) ((ByteCodeVal*) &stack->data)->i;
                stack->data = *(void**) &fv;
            }
            break;
        case 9:
            HSD_ASSERT(387, stack);
            fv = -(((ByteCodeVal*) &stack->data)->f);
            stack->data = *(void**) &fv;
            break;
        case 0x0A:
            HSD_ASSERT(393, stack);
            ((ByteCodeVal*) &stack->data)->i =
                -((ByteCodeVal*) &stack->data)->i;
            break;
        case 0x0B:
            HSD_ASSERT(399, stack);
            ((ByteCodeVal*) &stack->data)->i = HSD_Randi(2);
            break;
        case 0x0C:
            HSD_ASSERT(405, stack);
            fv = HSD_Randf();
            stack->data = *(void**) &fv;
            break;
        case 0x0D:
            HSD_ASSERT(411, stack);
            fv = (f32) sin(
                (f32) (DEG_TO_RAD * (f64) ((ByteCodeVal*) &stack->data)->f));
            stack->data = *(void**) &fv;
            break;
        case 0x0E:
            HSD_ASSERT(417, stack);
            fv = (f32) cos(
                (f32) (DEG_TO_RAD * (f64) ((ByteCodeVal*) &stack->data)->f));
            stack->data = *(void**) &fv;
            break;
        case 0x0F:
            HSD_ASSERT(423, stack);
            fv = (f32) tan(
                (f32) (DEG_TO_RAD * (f64) ((ByteCodeVal*) &stack->data)->f));
            stack->data = *(void**) &fv;
            break;
        case 0x10:
            HSD_ASSERT(429, stack);
            fv = (f32) (RAD_TO_DEG * (f32) asin(((ByteCodeVal*) &stack->data)->f));
            stack->data = *(void**) &fv;
            break;
        case 0x11:
            HSD_ASSERT(435, stack);
            fv = (f32) (RAD_TO_DEG * (f32) acos(((ByteCodeVal*) &stack->data)->f));
            stack->data = *(void**) &fv;
            break;
        case 0x12:
            HSD_ASSERT(441, stack);
            fv = (f32) (RAD_TO_DEG * (f32) atan(((ByteCodeVal*) &stack->data)->f));
            stack->data = *(void**) &fv;
            break;
        case 0x13:
            HSD_ASSERT(447, stack);
            fv = (f32) log(((ByteCodeVal*) &stack->data)->f);
            stack->data = *(void**) &fv;
            break;
        case 0x14:
            HSD_ASSERT(453, stack);
            fv = (f32) exp(((ByteCodeVal*) &stack->data)->f);
            stack->data = *(void**) &fv;
            break;
        case 0x15:
            HSD_ASSERT(459, stack);
            if (((ByteCodeVal*) &stack->data)->f < 0.0F) {
                fv = -(((ByteCodeVal*) &stack->data)->f);
                stack->data = *(void**) &fv;
            }
            break;
        case 0x28:
            HSD_ASSERT(467, stack);
            {
                d0 = ((ByteCodeVal*) &stack->data)->i;
                if (d0 < 0) {
                    ((ByteCodeVal*) &stack->data)->i = -d0;
                }
            }
            break;
        case 0x16:
            HSD_ASSERT(474, stack);
            fv = sqrtf(((ByteCodeVal*) &stack->data)->f);
            stack->data = *(void**) &fv;
            break;
        case 0x31:
            HSD_ASSERT(480, stack);
            stack->data = (void*) !(s32) stack->data;
            break;
        case 0x17:
            HSD_ASSERT(501, stack);
            HSD_ASSERT(501, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            fv = ((ByteCodeVal*) &stack->data)->f + f0;
            stack->data = *(void**) &fv;
            break;
        case 0x18:
            HSD_ASSERT(507, stack);
            HSD_ASSERT(507, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            fv = ((ByteCodeVal*) &stack->data)->f - f0;
            stack->data = *(void**) &fv;
            break;
        case 0x19:
            HSD_ASSERT(513, stack);
            HSD_ASSERT(513, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            fv = ((ByteCodeVal*) &stack->data)->f * f0;
            stack->data = *(void**) &fv;
            break;
        case 0x1A:
            HSD_ASSERT(519, stack);
            HSD_ASSERT(519, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            fv = ((ByteCodeVal*) &stack->data)->f / f0;
            stack->data = *(void**) &fv;
            break;
        case 0x1B:
            HSD_ASSERT(525, stack);
            HSD_ASSERT(525, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 =
#ifdef MUST_MATCH
                f1 =
#endif
                    ((ByteCodeVal*) &stack->data)->f;
            fv = (f32) fmod(f1, f0);
            stack->data = *(void**) &fv;
            break;
        case 0x1C:
            HSD_ASSERT(531, stack);
            HSD_ASSERT(531, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i + d1);
            break;
        case 0x1D:
            HSD_ASSERT(536, stack);
            HSD_ASSERT(536, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i - d1);
            break;
        case 0x1E:
            HSD_ASSERT(541, stack);
            HSD_ASSERT(541, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i * d1);
            break;
        case 0x1F:
            HSD_ASSERT(546, stack);
            HSD_ASSERT(546, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i / d1);
            break;
        case 0x20:
            HSD_ASSERT(551, stack);
            HSD_ASSERT(551, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            d0 = ((ByteCodeVal*) &stack->data)->i;
            ((ByteCodeVal*) &stack->data)->i = (d0 % d1);
            break;
        case 0x21:
            HSD_ASSERT(556, stack);
            HSD_ASSERT(556, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            fv = (f32) pow(((ByteCodeVal*) &stack->data)->f, f0);
            stack->data = *(void**) &fv;
            break;
        case 0x22:
            HSD_ASSERT(562, stack);
            HSD_ASSERT(562, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            if (((ByteCodeVal*) &stack->data)->f > f0) {
                stack->data = *(void**) &f0;
            }
            break;
        case 0x23:
            HSD_ASSERT(569, stack);
            HSD_ASSERT(569, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            if (((ByteCodeVal*) &stack->data)->f < f0) {
                stack->data = *(void**) &f0;
            }
            break;
        case 0x24:
            HSD_ASSERT(576, stack);
            HSD_ASSERT(576, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            if (((ByteCodeVal*) &stack->data)->i > d1) {
                ((ByteCodeVal*) &stack->data)->i = d1;
            }
            break;
        case 0x25:
            HSD_ASSERT(583, stack);
            HSD_ASSERT(583, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            if (((ByteCodeVal*) &stack->data)->i < d1) {
                ((ByteCodeVal*) &stack->data)->i = d1;
            }
            break;
        case 0x26:
            HSD_ASSERT(590, stack);
            HSD_ASSERT(590, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 = ((ByteCodeVal*) &stack->data)->f;
            /* Custom Robo: wowjinxy/KAR bytecode.c's form (the right angle in radians, then scaled with the atan2 result;
             * main.dol's code), with MSL's double atan2 */
            {
                f32 result;
                f32 x = f0;
                if (x == 0.0F) {
                    result = f1 >= 0.0F ? 1.5707964F : -1.5707964F;
                } else {
                    result = (f32) atan2(f1, x);
                }
                fv = (f32) (RAD_TO_DEG * result);
            }
            stack->data = *(void**) &fv;
            break;
        case 0x33:
            HSD_ASSERT(596, stack);
            HSD_ASSERT(596, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 = ((ByteCodeVal*) &stack->data)->f;
            ((ByteCodeVal*) &stack->data)->i = (f1 < f0);
            break;
        case 0x34:
            HSD_ASSERT(601, stack);
            HSD_ASSERT(601, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 = ((ByteCodeVal*) &stack->data)->f;
            ((ByteCodeVal*) &stack->data)->i = (f1 > f0);
            break;
        case 0x35:
            HSD_ASSERT(606, stack);
            HSD_ASSERT(606, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->f <= f0);
            break;
        case 0x36:
            HSD_ASSERT(611, stack);
            HSD_ASSERT(611, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 = ((ByteCodeVal*) &stack->data)->f;
            ((ByteCodeVal*) &stack->data)->i = (f1 >= f0);
            break;
        case 0x37:
            HSD_ASSERT(616, stack);
            HSD_ASSERT(616, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 = ((ByteCodeVal*) &stack->data)->f;
            ((ByteCodeVal*) &stack->data)->i = (f1 == f0);
            break;
        case 0x38:
            HSD_ASSERT(621, stack);
            HSD_ASSERT(621, stack->next);
            f0 = ((ByteCodeVal*) &stack->data)->f;
            stack = HSD_SListRemove(stack);
            f1 = ((ByteCodeVal*) &stack->data)->f;
            ((ByteCodeVal*) &stack->data)->i = (f1 != f0);
            break;
        case 0x29:
            HSD_ASSERT(626, stack);
            HSD_ASSERT(626, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i < d1);
            break;
        case 0x2A:
            HSD_ASSERT(631, stack);
            HSD_ASSERT(631, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i > d1);
            break;
        case 0x2B:
            HSD_ASSERT(636, stack);
            HSD_ASSERT(636, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i <= d1);
            break;
        case 0x2C:
            HSD_ASSERT(641, stack);
            HSD_ASSERT(641, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i >= d1);
            break;
        case 0x2D:
            HSD_ASSERT(646, stack);
            HSD_ASSERT(646, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i == d1);
            break;
        case 0x2E:
            HSD_ASSERT(651, stack);
            HSD_ASSERT(651, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            ((ByteCodeVal*) &stack->data)->i =
                (((ByteCodeVal*) &stack->data)->i != d1);
            break;
        case 0x2F:
            HSD_ASSERT(656, stack);
            HSD_ASSERT(656, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            d0 = ((ByteCodeVal*) &stack->data)->i;
            {
                int val = 0;
                if (d0 != 0) {
                    if (d1 != 0) {
                        val = 1;
                    }
                }
                ((ByteCodeVal*) &stack->data)->i = val;
            }
            break;
        case 0x30:
            HSD_ASSERT(661, stack);
            HSD_ASSERT(661, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            d0 = ((ByteCodeVal*) &stack->data)->i;
            {
                int val = 0; /* Custom Robo: main.dol's form */
                if (d0 != 0 || d1 != 0) {
                    val = 1;
                }
                ((ByteCodeVal*) &stack->data)->i = val;
            }
            break;
        case 0x32:
            HSD_ASSERT(666, stack);
            HSD_ASSERT(666, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            d0 = ((ByteCodeVal*) &stack->data)->i;
            ((ByteCodeVal*) &stack->data)->i =
                (d0 == 0 && d1 != 0) || (d0 != 0 && d1 == 0);
            break;
        case 0x39:
            HSD_ASSERT(671, stack);
            HSD_ASSERT(671, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            stack->data = (void*) (((ByteCodeVal*) &stack->data)->i & d1);
            break;
        case 0x3A:
            HSD_ASSERT(676, stack);
            HSD_ASSERT(676, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            stack->data = (void*) (((ByteCodeVal*) &stack->data)->i | d1);
            break;
        case 0x3B:
            HSD_ASSERT(681, stack);
            HSD_ASSERT(681, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            stack->data = (void*) (((ByteCodeVal*) &stack->data)->i ^ d1);
            break;
        case 0x27:
            HSD_ASSERT(687, stack);
            HSD_ASSERT(687, stack->next);
            d1 = ((ByteCodeVal*) &stack->data)->i;
            stack = HSD_SListRemove(stack);
            d0 = ((ByteCodeVal*) &stack->data)->i;
            ((ByteCodeVal*) &stack->data)->i = (d0 + HSD_Randi((d1 - d0) + 1));
            break;
        default:
            OSReport("unexpected opcode 0x%x.\n", last_command);
            HSD_Panic(__FILE__, 693, ""); /* Custom Robo: main.dol passes 693 (wowjinxy/KAR), Melee 700 */
            break;
        }
    }
}
