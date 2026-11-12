/* fn_80075950: Nov 5, 2026 hand draft (m2c 86%): queues the five part files of the four robos (body, gun, bomb, pod, legs from cr_g_BattleLoadouts) into the slot table lbl_80377E60, a [5][4] array of cr_FileSlot pointers, each into its robo's buffer; every buffer address written as the first loop's form, `(void *) (buf + ((s32) (i * N * 4) / 4))` (the array form `&buf[(i << k) / 4]` schedules the store after the argument) */
#define CR_SLOTS ((cr_FileSlot *(*)[4]) lbl_80377E60)
extern s32 lbl_8044F2E4;

void fn_80075950(void) {
    s32 i;

    i = 0;
    while (i < 4) {
        CR_SLOTS[0][i] = cr_Archive_OpenEntry(0xF, cr_g_BattleLoadouts[i].parts[1]);
        cr_FileSlot_QueueRead(CR_SLOTS[0][i], (void *) (lbl_8044F2E4 + ((s32) (i * 0x16000 * 4) / 4)));
        i += 1;
    }
    i = 0;
    while (i < 4) {
        CR_SLOTS[1][i] = cr_Archive_OpenEntry(0x10, cr_g_BattleLoadouts[i].parts[2]);
        cr_FileSlot_QueueRead(CR_SLOTS[1][i], (void *) (lbl_8044F2E0 + ((s32) (i * 0x40000 * 4) / 4)));
        i += 1;
    }
    i = 0;
    while (i < 4) {
        CR_SLOTS[2][i] = cr_Archive_OpenEntry(0x11, cr_g_BattleLoadouts[i].parts[3]);
        cr_FileSlot_QueueRead(CR_SLOTS[2][i], (void *) (lbl_8044F2DC + ((s32) (i * 0x40000 * 4) / 4)));
        i += 1;
    }
    i = 0;
    while (i < 4) {
        CR_SLOTS[3][i] = cr_Archive_OpenEntry(0x16, cr_g_BattleLoadouts[i].parts[4]);
        cr_FileSlot_QueueRead(CR_SLOTS[3][i], (void *) (lbl_8044F2CC + ((s32) (i * 0x1000 * 4) / 4)));
        i += 1;
    }
    i = 0;
    while (i < 4) {
        CR_SLOTS[4][i] = cr_Archive_OpenEntry(0x12, cr_g_BattleLoadouts[i].parts[0]);
        cr_FileSlot_QueueRead(CR_SLOTS[4][i], (void *) (lbl_8044F2D8 + ((s32) (i * 0x14000 * 4) / 4)));
        i += 1;
    }
}
