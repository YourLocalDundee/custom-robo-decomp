/* fn_80021350: Nov 3, 2026 hand draft (m2c 95%): the queue head advanced before the current read is taken (as fn_800210B0), and an empty statement in the early return (`(void) 0;`, a compiled-out debug macro) gives the original's `bne +8; b end` */

void fn_80021350(s32 result, DVDFileInfo *fileInfo) {
    cr_FileSlot *var_r31;
    s32 var_r30;
    s32 var_r29;

    if (result == -1) {
        (void) 0;
        return;
    }
    cr_g_FileSysReady.queue_head += 1;
    cr_g_FileSysReady.queue_head &= 0x1F;
    var_r31 = cr_g_FileSysReady.current_read;
    if ((s8) var_r31->in_use == -1) {
        var_r31->in_use = 0;
        var_r31->index = 0;
        var_r31->state = 1;
        var_r31->offset = 0;
        var_r31->sectors = 0;
        var_r31->size = 0;
        DVDClose(&var_r31->file_info);
    } else {
        var_r31->read_done += var_r31->read_chunk;
        if ((s32) var_r31->read_done >= (s32) var_r31->read_left) {
            var_r31->unk_38 = OSGetTick();
            var_r31->state = 3;
        } else {
            cr_g_FileSysReady.queue[cr_g_FileSysReady.queue_tail] = cr_g_FileSysReady.current_read;
            var_r31->state = 5;
            var_r31->read_buffer = &var_r31->read_buffer[var_r31->read_chunk];
            var_r31->offset += var_r31->read_chunk;
            cr_g_FileSysReady.queue_tail += 1;
            cr_g_FileSysReady.queue_tail &= 0x1F;
        }
    }
    while ((u8) cr_g_FileSysReady.queue_head != (u8) cr_g_FileSysReady.queue_tail) {
        var_r31 = cr_g_FileSysReady.queue[cr_g_FileSysReady.queue_head];
        if ((s8) var_r31->in_use != -1) {
            cr_g_FileSysReady.current_read = var_r31;
            var_r30 = var_r31->read_left - var_r31->read_done;
            if (var_r30 > (s32) (cr_g_FileSysReady.chunk_sectors << 0xB)) {
                var_r29 = cr_g_FileSysReady.chunk_sectors << 0xB;
            } else {
                var_r29 = var_r30;
            }
            var_r31->read_chunk = (u32) var_r29;
            var_r31->state = 2;
            DVDReadAsyncPrio(&var_r31->file_info, var_r31->read_buffer, (s32) var_r31->read_chunk, (s32) var_r31->offset, fn_80021350, 2);
            return;
        }
        var_r31->in_use = 0;
        var_r31->index = 0;
        var_r31->state = 1;
        var_r31->offset = 0;
        var_r31->sectors = 0;
        var_r31->size = 0;
        DVDClose(&var_r31->file_info);
        cr_g_FileSysReady.queue_head += 1;
        cr_g_FileSysReady.queue_head &= 0x1F;
    }
    return;
}
