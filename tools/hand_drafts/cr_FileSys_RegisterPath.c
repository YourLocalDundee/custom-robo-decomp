/* cr_FileSys_RegisterPath: Nov 14, 2026 hand draft: the path lookup in the directory tree: the path upper-cased, split at each separator (0x5C or '/'), each directory found among its siblings by name, then the file in that directory's list; returns the size (u32) or 0; the tree nodes typed; the parameter used directly (m2c's copy of it, var_r28 = desc, makes MWCC spill r3 to its stack home: stw r3,8(r1); lwz r28,8(r1)) (47%) */

typedef struct cr_FileNode {
    char *name;
    s32 entry;
    u32 size;
    u32 unkC;
    struct cr_FileNode *next;
} cr_FileNode;

typedef struct cr_DirNode {
    char *name;
    cr_FileNode *files;
    struct cr_DirNode *next;
    u32 unkC;
    struct cr_DirNode *child;
} cr_DirNode;

u32 cr_FileSys_RegisterPath(cr_FileDesc *desc) {
    char path[0x100];
    char name[0x100];
    u32 i;
    cr_DirNode *dir;
    cr_FileNode *f;
    s32 more;

    sprintf(path, desc->path);
    for (i = 0; i < strlen(path); i++) {
        if (path[i] >= 'a' && path[i] <= 'z') {
            path[i] = path[i] - 0x20;
        }
    }
    path[i] = 0;
    dir = (cr_DirNode *) &cr_g_DirTree;
    do {
        more = 0;
        name[0] = 0;
        for (i = 0; i < strlen(path); i++) {
            name[i] = path[i];
            if (name[i] == 0x5C || name[i] == '/') {
                name[i] = 0;
                sprintf(path, &path[i + 1]);
                more = 1;
                if (strlen(name) != 0) {
                    while (dir != ((void *) 0)) {
                        if (strcmp(dir->name, name) == 0) {
                            dir = dir->child;
                            break;
                        }
                        dir = dir->next;
                    }
                }
                break;
            }
        }
    } while (more);
    if (dir == ((void *) 0)) {
        return 0;
    }
    for (f = dir->files; f != ((void *) 0); f = f->next) {
        if (strcmp(f->name, path) == 0) {
            desc->dvd_entry = f->entry;
            desc->size = f->size;
            return desc->size;
        }
    }
    return 0;
}
