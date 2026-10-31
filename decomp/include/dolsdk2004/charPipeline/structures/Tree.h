/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/charPipeline/structures/Tree.h */ #ifndef _CHARPIPELINE_STRUCTURES_TREE_H_
#define _CHARPIPELINE_STRUCTURES_TREE_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  {
    Ptr Prev;
    Ptr Next;
    Ptr Parent;
    Ptr Children;
} DSBranch, *DSBranchPtr;

typedef struct  {
    u32 Offset;
    Ptr Root;
} DSTree, *DSTreePtr;

void DSExtractBranch(DSTreePtr tree, Ptr obj);
void DSInitTree(DSTreePtr tree, Ptr obj, DSBranchPtr branch);
void DSInsertBranchBelow(DSTreePtr tree, Ptr cursor, Ptr obj);
void DSInsertBranchBeside(DSTreePtr tree, Ptr cursor, Ptr obj);
void DSRemoveBranch(DSTreePtr tree, Ptr obj);

#ifdef __cplusplus
}
#endif

#endif
