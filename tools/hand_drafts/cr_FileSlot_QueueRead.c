/* cr_FileSlot_QueueRead: hand-written from main.dol's asm on its m2c draft (step 109b): the buffer parameter used itself (m2c's sp8 copy took a saved register) and the function returns 0 (li r3, 0); 106 of 116: the queue address loads are scheduled in another order */

s32 cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer) {
    u32 temp_r30_10;
    u32 var_r29_65;

    temp_r30_10 = slot->size;
    OSDisableInterrupts();
    slot->read_left = temp_r30_10;
    slot->read_done = 0;
    slot->read_buffer = (u8 *) buffer;
    slot->unk_28 = 0;
    slot->start_tick = OSGetTick();
    slot->unk_38 = 0;
    slot->read_total = temp_r30_10;
    slot->unk_30 = -1;
    cr_g_FileSysReady.queue[cr_g_FileSysReady.queue_tail] = slot;
    if ((u8) cr_g_FileSysReady.queue_head == (u8) cr_g_FileSysReady.queue_tail) {
        cr_g_FileSysReady.queue_tail += 1;
        cr_g_FileSysReady.current_read = slot;
        cr_g_FileSysReady.queue_tail &= 0x1F;
        if ((s32) temp_r30_10 > (s32) (cr_g_FileSysReady.chunk_sectors << 0xB)) {
            var_r29_65 = cr_g_FileSysReady.chunk_sectors << 0xB;
        } else {
            var_r29_65 = temp_r30_10;
        }
        slot->read_chunk = var_r29_65;
        slot->state = 2;
        OSEnableInterrupts();
        if ((lbl_8044F6C4[1] + M2C_FIELD(lbl_8044F6C4, u8 *, 0)) != 0) {
            HSD_DevComRequest(slot->dvd_entry, slot->offset, (u32) slot->read_buffer, slot->read_chunk, 0x21, 1, (void (*)(int, unsigned int, void *, int)) fn_800210B0, (unsigned int) ((void (*)(int, unsigned int, void *, int)) ((u32) slot)));
        } else {
            DVDReadAsyncPrio(&slot->file_info, slot->read_buffer, (s32) slot->read_chunk, (s32) slot->offset, fn_80021350, 2);
        }
    } else {
        cr_g_FileSysReady.queue_tail += 1;
        cr_g_FileSysReady.queue_tail &= 0x1F;
        slot->state = 6;
        OSEnableInterrupts();
    }
    return 0;
}
