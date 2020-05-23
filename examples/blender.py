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
        
    # Texture coordinates per vertex *per polygon loop*.
    # Create UV coordinate layer and set values
    if _mesh.haveTexCoords():
        uv_layer = mesh.uv_layers.new()
        for i, uv in enumerate(uv_layer.data):
            st = _mesh.getTexCoord( _mesh.getFaceIndex(i) )
            uv.uv = [st.x, st.y]

    # Vertex color per vertex *per polygon loop*    
    # Create vertex color layer and set values
    if _mesh.haveColors():
        vcol_lay = mesh.vertex_colors.new()
        for i, col in enumerate(vcol_lay.data):
            color = _mesh.getColor( _mesh.getFaceIndex(i) )
            col.color[0] = color.r
            col.color[1] = color.g
            col.color[2] = color.b
            col.color[3] = color.a
            
    # We're done setting up the mesh values, update mesh object and 
    # let Blender do some checks on it
        
    mesh.update()
    mesh.validate()

heightmap = Image();
loadPng("gale.png", heightmap, 1)
terrain = toTerrain(heightmap, 100.0, 0.005, 1.0)
center(terrain)
scale(terrain, 0.01)
add_mesh("terrain", terrain)