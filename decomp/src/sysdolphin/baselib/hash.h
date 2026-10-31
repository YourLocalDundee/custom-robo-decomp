/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/hash.h */ #ifndef _hash_h_
#define _hash_h_

#include <Runtime/platform.h>

#include <sysdolphin/baselib/forward.h>

#include <sysdolphin/baselib/class.h>

#define hash(s) (s % 0x65)

struct HSD_HashEntry {
    HSD_HashEntry* next;
    void* key;
    void* value;
};

typedef struct _HSD_HashClass {
    struct _HSD_HashClassInfo* class_info;
} HSD_HashClass;

typedef struct _HSD_HashClassInfo {
    HSD_ClassInfo parent;
    int (*getidx)(HSD_Hash* hash, void* key); /* Custom Robo: the key is passed (main.dol keeps it in r4 for the call) */
    bool (*keycheck)(HSD_Hash* hash, void* table_key, void* key);
} HSD_HashClassInfo;

struct HSD_Hash {
    HSD_HashClass parent;
    HSD_HashEntry** table;
    u32 table_size;
};

/* Custom Robo: HashSearchEntry is a static inline in this revision (wowjinxy/KAR hash.c; main.dol has no out-of-line copy) */
void* HSD_HashSearch(HSD_Hash*, void*, int*);

#endif
