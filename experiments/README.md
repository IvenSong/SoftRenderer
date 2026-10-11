# 贴图立方体与方向光：主场景与回归实验

> 当前主场景已切换为 [Blender 小房子导入实验](blender_house.md)，由 `prepare.cpp` 加载 OBJ 并交给 `main.cpp` 运行。2026-09-26 已移除 `aiProcess_FlipUVs` 并重新验证，贴图位置恢复正确。下文的立方体主场景描述和测量数据为此前记录；独立立方体对照实验仍保留。

实验已接入主程序。运行 `softRenderer` 会显示使用本地图片的立方体，立方体绕 Y 轴缓慢旋转；WASD 移动相机，按住鼠标右键拖动调整朝向。

## 代码入口

- [prepare.cpp](../prepare.cpp)：加载纹理、设置方向光与环境光、创建 VAO/VBO/EBO、初始化相机及逐帧绘制。
- [cubeMesh.h](../cubeMesh.h)：主场景和实验共用的 24 个顶点、36 个索引；每面独立法线与 UV。
- [LightShader](../gpu/Shader/lightShader.cpp)：世界空间法线变换、漫反射与环境光。
- [cube_lighting.cpp](cube_lighting.cpp)：离屏渲染、主场景集成验证和回归检查。

## 场景参数

| 项目 | 配置 |
| --- | --- |
| 几何 | 原点处边长 1.6 的立方体，12 个 CCW 三角形 |
| 纹理 | `assets/texture/img1.jpg`，每面铺满一次，最近邻采样 |
| 相机 | 从 `(3, 2.5, 4)` 看向原点，50° 视场角，近远面 0.1 / 100 |
| 方向光 | 白色，光线传播方向 `normalize(-0.4, -1, -0.6)` |
| 环境光 | `(0.15, 0.15, 0.15)` |
| 光栅状态 | 背面剔除开启，深度测试 `DEPTH_LESS` |
| 动画 | 主场景每帧绕 Y 轴增加 0.001 弧度；独立对照实验保持模型不动 |

最终 RGB 为 `纹理 RGB × (环境光 + 光色 × max(N·(-L), 0))`，透明度保留纹理 alpha。初始姿态下，顶面、正面和右面的亮度系数约为 0.961、0.637、0.474。

[主场景实际输出](../out/cube-lighting-experiment/cube_main_scene.png) · [光照对照实验](../out/cube-lighting-experiment/cube_light_actual.png) · [原始测量数据](../out/cube-lighting-experiment/measurements.txt)

## 修复与验证

| 问题 | 修复 | 验证结果 |
| --- | --- | --- |
| 三角形光栅化丢失法线 | 对法线进行重心插值，再由 GPU 恢复透视修正 | 57,971 个片元中零法线和非有限法线均为 0 |
| 清屏未重置深度 | `FrameBuffer::clear()` 同时将深度恢复为 1 | 连续两帧光照输出逐像素一致；贴图两帧均覆盖 57,971 像素 |
| 环境光重复计算漫反射 | 使用 `mEnvLight`，与漫反射相加后乘纹理 | 灰度 128 的探针输出依次为 32、96、64，与预期一致 |
| 法线未随模型变换 | 采用模型矩阵的逆转置，并归一化 | Y 轴旋转 90° 后法线约为 `(1,0,0)`；非均匀缩放测试通过 |
| 颜色限幅结果未用于输出 | 使用限制后的 `c` 转换颜色通道 | 输入 `(1.4,-0.25,0.5,1.2)` 得到 `(255,0,127,255)` |
| `app` 宏污染标准库名称 | 移除宏，主函数使用局部 Application 指针 | 编译中显式检查 `std::ios_base::app` 可正常使用 |

同时验证了零法线输入保留环境光、光照不改变纹理 alpha。相机初始化同步 pitch/yaw，首次鼠标操作不会跳变。

主场景通过与窗口程序完全相同的 `PreparedScene` 代码离屏绘制；前两帧分别产生 57,971 和 57,967 个非黑像素，数量的微小变化来自旋转。完整 Debug 构建成功，实验输出 `ALL_CHECKS_PASSED`，并已检查生成图片。未进行窗口内的人工交互测试。

修复前的图片与测量数据保存在 [before](../out/cube-lighting-experiment/before/measurements.txt)，可比较 [原光照结果](../out/cube-lighting-experiment/before/cube_light_actual.png) 和 [原第二帧结果](../out/cube-lighting-experiment/before/cube_texture_frame2.png)。

## 运行

在 Visual Studio 开发者终端、项目根目录下：

```powershell
cmake --build out/build/x64-Debug
./out/build/x64-Debug/softRenderer.exe
```

执行回归实验并导出图片：

```powershell
./experiments/run_cube_lighting.ps1
```

实验程序将 BMP、PNG 和 `measurements.txt` 输出到 `out/cube-lighting-experiment`。PNG 由真实帧缓冲 BMP 无损转换而来。任一回归检查失败时，程序以非零状态退出。

脚本支持 `-Texture`、`-BuildDirectory` 和 `-OutputDirectory`；`-Texture` 影响独立对照实验，主场景集成检查使用 `prepare.cpp` 配置的纹理。
