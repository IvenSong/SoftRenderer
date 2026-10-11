# 最小带贴图模型导入实验

> 以下为此前的单三角形实验记录。当前 `prepare.cpp` 和 `modelImportExperiment` 已切换到 [Blender 小房子实验](blender_house.md)，运行结果及命令以该报告为准。

资源：`assets/model/minimal_triangle/triangle.obj`、`triangle.mtl`、`diffuse.jpg`。
模型只有一个三角形，包含 UV 和法线；贴图复制自项目已有的 `assets/texture/img1.jpg`。

场景入口：`prepare.cpp`。`PreparedScene::initialize()` 加载模型、设置相机与全环境光，`render()` 绘制静止的带贴图三角形。`main.cpp` 已通过这两个函数运行场景，WASD 和鼠标右键相机控制仍可使用。

验证入口：`experiments/model_import.cpp`。先检查资源，再调用与 main 相同的 `PreparedScene`，向 128×128 离屏缓冲绘制一帧。程序退出码为 0 且有非黑像素即通过。

在 Visual Studio x64 开发者终端中，从项目根目录执行：

```powershell
cmake --build out/build/x64-Debug --target softRenderer modelImportExperiment
./out/build/x64-Debug/experiments/modelImportExperiment.exe
./out/build/x64-Debug/softRenderer.exe
```

2026-09-25 实测：Debug 编译成功，运行退出码 0。

```text
ASSET_OK: meshes=1 faces=1 root_children=1 texture=960x1920
PREPARE_BEGIN
PREPARE_OK; DRAW_BEGIN
DRAW_END: colored_pixels=2592
```

接入后的构建和运行日志保存在 `out/model-prepare-build.log` 和 `out/model-prepare-run.log`。
主程序编译成功；从主程序输出目录运行离屏实验成功，也验证了构建时复制的资源可用。未进行窗口交互测试，没有修改 Model 或 GPU 实现。
