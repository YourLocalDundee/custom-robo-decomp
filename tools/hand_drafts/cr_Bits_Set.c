/* cr_Bits_Set: Nov 13, 2026 hand draft (m2c 33-40%): the word index and the mask are named locals (`u32 word = bit >> 5; u32 mask = 1 << (bit & 0x1F);`), which MWCC keeps in r31 / r30 even in this leaf function; the array index set[word] is computed again from bit */

void cr_Bits_Set(u32 *set, u32 bit) {
    u32 word = bit >> 5;
    u32 mask = 1 << (bit & 0x1F);

    set[word] |= mask;
}
