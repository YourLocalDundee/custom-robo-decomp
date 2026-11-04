STORES = ''.join('    LObjActiveLights[%d] = NULL;\n' % k for k in range(9))
PSTORES = ''.join('    lights[%d] = NULL;\n' % k for k in range(9))
DECL = ('    MtxPtr vmtx;\n    s32 i, num, idx;\n    HSD_SList* list;\n',
        '    MtxPtr vmtx;\n    s32 i, num, idx;\n    HSD_SList* list;\n    HSD_LObj** lights = LObjActiveLights;\n')
DECL_FIRST = ('    MtxPtr vmtx;\n    s32 i, num, idx;\n    HSD_SList* list;\n',
              '    HSD_LObj** lights = LObjActiveLights;\n    MtxPtr vmtx;\n    s32 i, num, idx;\n    HSD_SList* list;\n')
L3 = '    for (i = 0; idx < CR_LOBJ_MAX_GXLIGHT - 1 && i < num; i++) {\n        HSD_LObj* lobj = HSD_LObjGetActiveByIndex(i);'
L3P = '    for (i = 0; idx < CR_LOBJ_MAX_GXLIGHT - 1 && i < num; i++) {\n        HSD_LObj* lobj = (i >= 0 && i < 8) ? lights[i] : NULL;'
L4 = '    for (i = 0; i < num; i++) {\n        HSD_LObj* lobj = HSD_LObjGetActiveByIndex(i);'
L4P = '    for (i = 0; i < num; i++) {\n        HSD_LObj* lobj = (i >= 0 && i < 8) ? lights[i] : NULL;'
V = {
    'ptr_stores': [DECL, (STORES, PSTORES)],
    'ptr_all': [DECL, (STORES, PSTORES), (L3, L3P), (L4, L4P)],
    'ptr_loops': [DECL, (L3, L3P), (L4, L4P)],
    'ptr_first_all': [DECL_FIRST, (STORES, PSTORES), (L3, L3P), (L4, L4P)],
    'ptr_l3': [DECL, (STORES, PSTORES), (L3, L3P)],
}
