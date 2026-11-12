/* the battle action file's .bss pool at 0x80329B60 (0xA84 bytes): file statics, laid out by MWCC in the order of
   their first reference in the file; cr_Pool9B60_Order (scoring only) references them in address order first */
static u8 cr_s_9B60_000[0xA0C];
static Vec cr_s_9B60_A0C;
static Vec cr_s_9B60_A18;
static Vec cr_s_9B60_A24;
static Vec cr_s_9B60_A30;
static Vec cr_s_9B60_A3C;
static Vec cr_s_9B60_A48;
static Vec cr_s_9B60_A54;
static Vec cr_s_9B60_A60;
static Vec cr_s_9B60_A6C;
static Vec cr_s_9B60_A78;
extern void cr_Pool9B60_Use(void *);
void cr_Pool9B60_Order(void) {
    cr_Pool9B60_Use(cr_s_9B60_000);
    cr_Pool9B60_Use(&cr_s_9B60_A0C);
    cr_Pool9B60_Use(&cr_s_9B60_A18);
    cr_Pool9B60_Use(&cr_s_9B60_A24);
    cr_Pool9B60_Use(&cr_s_9B60_A30);
    cr_Pool9B60_Use(&cr_s_9B60_A3C);
    cr_Pool9B60_Use(&cr_s_9B60_A48);
    cr_Pool9B60_Use(&cr_s_9B60_A54);
    cr_Pool9B60_Use(&cr_s_9B60_A60);
    cr_Pool9B60_Use(&cr_s_9B60_A6C);
    cr_Pool9B60_Use(&cr_s_9B60_A78);
}

