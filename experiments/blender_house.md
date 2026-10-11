# Blender 小房子导出与软光栅读取实验

当前结果（2026-09-26）：按用户要求移除 `application/model.cpp` 和实验预检查中的 `aiProcess_FlipUVs`。Debug 主程序及实验程序重新构建成功；运行共用 prepare 的离屏实验，退出码 0，绘制 39,133 个非黑像素。门窗、屋顶瓦片和烟囱砖纹已回到正确位置。

与 Blender 原始参考的 RGB 平均绝对误差从 88.2225 降到 4.8126（0–255）。实际输出见 `out/blender-house-experiment/soft_renderer.png`，新对照图见 `out/blender-house-experiment/comparison.png`。修复前的图片、测量数据和日志保存在 `out/blender-house-experiment/before_uv_fix`。仍未进行窗口交互测试。

以下为修复前的诊断记录。

## 资源与接入

使用本机 Blender 4.1.1 的后台 Python API 建造小房子和烟囱，创建 UV、材质及 512×512 PNG 图集，再由 Blender 的 OBJ 导出器导出。未手写替代 OBJ。

- `assets/model/blender_house/house.blend`：可编辑的 Blender 工程，相对路径引用图集。
- `assets/model/blender_house/house.obj`、`house.mtl`：Y 向上、-Z 向前，三角化导出，包含 UV、法线和漫反射贴图引用。
- `assets/model/blender_house/house_atlas.png`：程序绘制的门窗、红瓦和砖纹图集。
- 2 个网格：House 16 个三角形，Chimney 12 个三角形，共 28 个。
- `prepare.cpp` 已切换到该模型，`main.cpp` 沿用 `PreparedScene` 调用。相机为 `(3,2.7,4.5)`，看向 `(0,0.25,0)`，垂直视场角 50°，全环境光。

## 实测结果

Debug 主程序和实验程序构建成功。实验调用与 main 相同的 prepare，640×480 离屏渲染输出如下，退出码为 0：

```text
MESH_OK: House faces=16 texture=house_atlas.png
MESH_OK: Chimney faces=12 texture=house_atlas.png
ASSET_OK: meshes=2 faces=28 root_children=2 texture=512x512
PREPARE_BEGIN
PREPARE_OK; DRAW_BEGIN
DRAW_END: colored_pixels=39133
```

这个退出码只表示资源结构检查和渲染冒烟实验通过，不表示画面与 Blender 一致。此次未测试窗口交互。

![修复前 Blender 与软光栅对照](../out/blender-house-experiment/before_uv_fix/comparison.png)

左边是 Blender 原始参考；右边是当前软光栅实际帧缓冲。轮廓交并比为 0.980359，几何基本一致，但软光栅把门窗贴到了屋顶、烟囱上，把瓦片和砖纹贴到了墙面上。

诊断时仅在 Blender 内存场景中执行 `v = 1-v`，不保存、不重新导出资产。软光栅与原始参考的 RGB 平均绝对误差为 88.2225，与翻转 V 后参考的误差降为 5.6861（0–255，共同前景区域）。不同光栅规则和抗锯齿会产生少量剩余差异。

代码对应位置：`application/model.cpp:88` 使用 `aiProcess_FlipUVs`，`application/image.cpp:26` 使用 `stbi_set_flip_vertically_on_load(true)`。这套 UV/图片纵向约定与 Blender 导出的 OBJ 不匹配。按用户要求未修改 Model、image、Texture、shader 或 GPU 实现，也未翻转正式资产来掩盖问题。

## 复现

从项目根目录运行，在 Visual Studio x64 开发者终端中构建：

```powershell
# 重新生成 Blender 工程、OBJ/MTL、图集和原始参考图
& 'C:/Program Files/Blender Foundation/Blender 4.1/blender.exe' --background --factory-startup --python experiments/create_blender_house.py
cmake --build out/build/x64-Debug --target softRenderer modelImportExperiment
./out/build/x64-Debug/experiments/modelImportExperiment.exe
# 可选：重做 UV 翻转诊断和图像对比
& 'C:/Program Files/Blender Foundation/Blender 4.1/blender.exe' --background --factory-startup --python experiments/check_blender_house_uv.py
./experiments/compare_blender_house.ps1
# 运行主程序
./out/build/x64-Debug/softRenderer.exe
```

输出在 `out/blender-house-experiment`：`run.log`、`exit.txt`、`soft_renderer.bmp/png`、`blender_reference.png`、`blender_v_flipped.png`、`comparison.png`、`comparison.json`。构建日志为 `out/blender-house-build.log`。此前的单三角形资源仍保留在 `assets/model/minimal_triangle`。
