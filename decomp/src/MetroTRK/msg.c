/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msg.c */ /**
 * msg.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.h"
#include "TRK_MINNOW_DOLPHIN/utils/common/MWTrace.h"
#include "trk.h"

DSError TRKMessageSend(TRK_Msg* msg) {
    DSError write_err = TRKWriteUARTN(&msg->m_msg, msg->m_msgLength);
    MWTRACE(1, "MessageSend : cc_write returned %ld\n", write_err);
    return DS_NoError;
}
