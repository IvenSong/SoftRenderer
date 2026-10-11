"""Diagnostic reference only: flip V in memory, never save the asset or fix the loader."""
from pathlib import Path
import bpy

root = Path(__file__).resolve().parents[1]
bpy.ops.wm.open_mainfile(filepath=str(root / "assets/model/blender_house/house.blend"))
for obj in bpy.context.scene.objects:
    if obj.type == "MESH":
        for loop in obj.data.uv_layers.active.data:
            loop.uv.y = 1.0 - loop.uv.y
bpy.context.scene.render.filepath = str(root / "out/blender-house-experiment/blender_v_flipped.png")
bpy.ops.render.render(write_still=True)
print("DIAGNOSTIC_REFERENCE_SAVED; original asset unchanged", flush=True)
