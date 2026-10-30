/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.h */ #ifndef METROTRK_PORTABLE_MSG_H
#define METROTRK_PORTABLE_MSG_H

#include <types.h>

typedef struct _TRK_Msg {
	u8 _00[8];
	u32 m_msgLength;
	u32 _0C;
	u32 m_msg;
} TRK_Msg;

#endif /* METROTRK_PORTABLE_MSG_H */
