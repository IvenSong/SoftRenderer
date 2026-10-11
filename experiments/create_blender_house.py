"""Run with Blender --background --factory-startup --python this_file.py."""
from pathlib import Path
import json
import math
import struct
import zlib

import bpy
import bmesh
import numpy as np
from mathutils import Vector

ROOT = Path(__file__).resolve().parents[1]
ASSET = ROOT / "assets/model/blender_house"
OUTPUT = ROOT / "out/blender-house-experiment"
ASSET.mkdir(parents=True, exist_ok=True)
OUTPUT.mkdir(parents=True, exist_ok=True)
bpy.ops.object.select_all(action="SELECT")
bpy.ops.object.delete(use_global=False)

# Deterministic diagnostic atlas: door/windows make orientation errors visible.
# Array rows are bottom to top, matching Blender UV coordinates.
pixels = np.zeros((512, 512, 3), dtype=np.uint8)
def rect(x0, y0, x1, y1, color):
    pixels[y0:y1, x0:x1] = color

rect(0, 0, 256, 256, (238, 204, 133))
rect(256, 0, 512, 256, (145, 199, 169))
rect(0, 256, 256, 512, (182, 63, 45))
rect(256, 256, 512, 512, (115, 83, 72))
for tile_x in (0, 256):
    for x in (32, 165):
        rect(tile_x+x-5, 130, tile_x+x+61, 213, (245, 242, 221))
        rect(tile_x+x, 135, tile_x+x+56, 208, (38, 110, 180))
        rect(tile_x+x+25, 135, tile_x+x+31, 208, (245, 242, 221))
        rect(tile_x+x, 168, tile_x+x+56, 174, (245, 242, 221))
rect(96, 4, 160, 116, (112, 62, 35))
rect(145, 54, 151, 61, (252, 220, 75))
for y in range(272, 512, 32):
    rect(0, y, 256, y+3, (105, 38, 30))
    for x in range((y//32 % 2)*16, 256, 32):
        rect(x, y, x+2, min(y+32, 512), (220, 101, 66))
for y in range(272, 512, 32):
    rect(256, y, 512, y+3, (210, 190, 159))
    for x in range(256+(y//32 % 2)*24, 512, 48):
        rect(x, y, x+3, min(y+32, 512), (210, 190, 159))

def chunk(kind, data):
    return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind+data) & 0xffffffff)

raw = b"".join(b"\x00" + row.tobytes() for row in pixels[::-1])
(ASSET / "house_atlas.png").write_bytes(
    b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", 512, 512, 8, 2, 0, 0, 0))
    + chunk(b"IDAT", zlib.compress(raw)) + chunk(b"IEND", b""))
texture = bpy.data.images.load(str(ASSET / "house_atlas.png"))
material = bpy.data.materials.new("HouseAtlas")
material.use_nodes = True
nodes = material.node_tree.nodes
bsdf = nodes.get("Principled BSDF")
image_node = nodes.new("ShaderNodeTexImage")
image_node.image = texture
material.node_tree.links.new(image_node.outputs["Color"], bsdf.inputs["Base Color"])
# Emission gives a reference with exactly the same unlit texture as mEnvLight=1.
material.node_tree.links.new(image_node.outputs["Color"], bsdf.inputs["Emission Color"])
bsdf.inputs["Emission Strength"].default_value = 1.0
bsdf.inputs["Roughness"].default_value = 1.0

def make_mesh(name, vertices, faces, tiles):
    mesh = bpy.data.meshes.new(name)
    mesh.from_pydata(vertices, [], faces)
    mesh.update()
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.collection.objects.link(obj)
    mesh.materials.append(material)
    uv = mesh.uv_layers.new(name="UVMap")
    for polygon, tile in zip(mesh.polygons, tiles):
        local_uv = [(0, 0), (1, 0), (1, 1), (0, 1)] if len(polygon.vertices) == 4 else [(0, .9), (1, .9), (.5, 1)]
        tx, ty = tile
        for loop, (u, v) in zip(polygon.loop_indices, local_uv):
            uv.data[loop].uv = ((tx+.02+.96*u)/2, (ty+.02+.96*v)/2)
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.recalc_face_normals(bm, faces=list(bm.faces))
    bm.to_mesh(mesh)
    bm.free()
    return obj

house = make_mesh("House", [
    (-1,-.75,-.8),(1,-.75,-.8),(1,.75,-.8),(-1,.75,-.8),
    (-1,-.75,.5),(1,-.75,.5),(1,.75,.5),(-1,.75,.5),
    (0,-.75,1.3),(0,.75,1.3)], [
    (0,1,5,4),(1,2,6,5),(2,3,7,6),(3,0,4,7),
    (4,5,8),(6,7,9),(5,6,9,8),(7,4,8,9),(3,2,1,0)],
    [(0,0),(1,0),(0,0),(1,0),(0,0),(0,0),(0,1),(0,1),(1,0)])
bpy.ops.mesh.primitive_cube_add(size=1, location=(.48,.2,1.3))
chimney = bpy.context.object
chimney.name = "Chimney"
chimney.dimensions = (.3,.35,1.0)
bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
chimney.data.materials.append(material)
for polygon in chimney.data.polygons:
    for loop, (u, v) in zip(polygon.loop_indices, [(0,0),(1,0),(1,1),(0,1)]):
        chimney.data.uv_layers.active.data[loop].uv = ((1+.02+.96*u)/2, (1+.02+.96*v)/2)

bpy.ops.object.select_all(action="DESELECT")
for obj in (house, chimney):
    obj.select_set(True)
bpy.context.view_layer.objects.active = house
bpy.ops.wm.obj_export(filepath=str(ASSET / "house.obj"),
    export_selected_objects=True, export_triangulated_mesh=True,
    export_uv=True, export_normals=True, export_materials=True,
    forward_axis="NEGATIVE_Z", up_axis="Y", path_mode="RELATIVE")

scene = bpy.context.scene
scene.render.engine = "CYCLES"
scene.cycles.device = "CPU"
scene.cycles.samples = 8
scene.world.color = (0, 0, 0)
scene.world.use_nodes = True
scene.world.node_tree.nodes.get("Background").inputs["Strength"].default_value = 0
scene.render.resolution_x = 640
scene.render.resolution_y = 480
scene.render.resolution_percentage = 100
scene.view_settings.view_transform = "Standard"
scene.view_settings.look = "None"
scene.view_settings.exposure = 0
scene.view_settings.gamma = 1
bpy.ops.object.camera_add(location=(3,-4.5,2.7))
camera = bpy.context.object
camera.rotation_euler = (Vector((0,0,.25))-camera.location).to_track_quat("-Z", "Y").to_euler()
camera.data.sensor_fit = "VERTICAL"
camera.data.sensor_height = 36
camera.data.lens = 18/math.tan(math.radians(25))
scene.camera = camera
scene.render.image_settings.file_format = "PNG"
scene.render.filepath = str(OUTPUT / "blender_reference.png")
texture.filepath = "//house_atlas.png"
bpy.context.preferences.filepaths.save_version = 0
bpy.ops.wm.save_as_mainfile(filepath=str(ASSET / "house.blend"))
bpy.ops.render.render(write_still=True)
(OUTPUT / "export_info.json").write_text(json.dumps({
    "blender": bpy.app.version_string, "meshes": 2, "triangles": 28,
    "texture": "house_atlas.png", "texture_size": [512,512],
    "renderer_camera": [3,2.7,4.5], "renderer_target": [0,.25,0],
    "fov_y": 50, "resolution": [640,480]}, indent=2), encoding="utf-8")
print("HOUSE_EXPORT_AND_REFERENCE_OK", flush=True)
