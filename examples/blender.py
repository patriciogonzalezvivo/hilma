import bpy
from hilma import Image, loadPng, toTerrain, center, scale

def add_mesh(name, _mesh, col_name="Collection"):   
    verts = []
    faces = []
    edges = []

    for v in _mesh.getVertices():
        verts.append( (v.x, v.y, v.z) )

    for t in _mesh.getTrianglesIndices():
        faces.append( (t.x, t.y, t.z ) )

    for e in _mesh.getLinesIndices():
        edges.append( (e.x, e.y) )

    mesh = bpy.data.meshes.new(name)
    obj = bpy.data.objects.new(mesh.name, mesh)
    col = bpy.data.collections.get(col_name)
    col.objects.link(obj)
    bpy.context.view_layer.objects.active = obj
    mesh.from_pydata(verts, edges, faces)

heightmap = Image();
loadPng("gale.png", heightmap, 1)
terrain = toTerrain(heightmap, 100.0, 0.001, 1.0)
center(terrain)
scale(terrain, 0.01)
add_mesh("terrain", terrain)