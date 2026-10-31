/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Export/targsupp.s */ .include "macros.inc"
# .file "targsupp.s"

.text
.balign 16

.fn TRKAccessFile, global
    twui r0, 0x0
    blr
.endfn TRKAccessFile

.fn TRKOpenFile, global
    twui r0, 0x0
    blr
.endfn TRKOpenFile

.fn TRKCloseFile, global
    twui r0, 0x0
    blr
.endfn TRKCloseFile

.fn TRKPositionFile, global
    twui r0, 0x0
    blr
.endfn TRKPositionFile
