import bpy, sys, math
from mathutils import Matrix
Ci = Matrix.Rotation(math.radians(-90), 4, 'X')
for p in sys.argv[sys.argv.index('--') + 1:]:
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bpy.ops.import_scene.gltf(filepath=p)
    pts = []
    for o in bpy.data.objects:
        if o.type == 'MESH':
            for o2 in [o]:
                me = o2.evaluated_get(bpy.context.evaluated_depsgraph_get()).to_mesh()
                pts += [Ci @ o2.matrix_world @ v.co for v in me.vertices]
    print('MP', p.split('scratchpad/')[-1][:45], len(pts))
    for x0 in [i * 0.5 for i in range(0, 12)]:
        s = [q for q in pts if x0 <= q.x < x0 + 0.5]
        if s:
            print('MP   x %.1f n %4d y %.2f..%.2f z %.2f..%.2f' % (x0, len(s), min(q.y for q in s), max(q.y for q in s), min(q.z for q in s), max(q.z for q in s)))
