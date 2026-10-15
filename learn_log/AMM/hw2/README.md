# 作业二：相机类编写与 YOLO 目标检测（AMM）

## 完成内容

| 文件 | 说明 |
|------|------|
| `io/my_camera.hpp` | `io::MyCamera` 类声明 + `io::CameraConfig` 参数结构体 |
| `io/my_camera.cpp` | 相机类实现，封装 hikrobot SDK 全部调用 |
| `main.cpp` | 主程序：初始化相机与 YOLO，循环读图 → 检测 → 绘制 → 显示 |
| `configs/camera.yaml` | 相机参数外置配置（曝光 / 增益 / 白平衡 / 帧率 / 翻转） |

## 构建与运行

这四个文件是 `hw2/homework/` 中缺失部分的补完，需放回模板目录后统一构建：

```bash
# 1. 拷贝到模板目录（覆盖空文件）
cp -r io configs main.cpp <仓库根目录>/hw2/homework/

# 2. 构建（依赖：OpenCV4 / OpenVINO 2024.6 / yaml-cpp / fmt / spdlog / Eigen3）
cd hw2/homework
mkdir build && cd build
cmake .. && make -j$(nproc)

# 3. 运行（需在 homework 目录下执行，yolo.yaml 中模型路径为相对路径 assets/）
cd .. && ./build/main
```

按 `q` 退出。

## 设计说明

- **RAII**：析构自动 `close()`，保证 `StopGrabbing / CloseDevice / DestroyHandle` 一定被调用，异常退出也不会占着相机。
- **头文件隔离**：`my_camera.hpp` 只前向声明 `struct _MV_FRAME_OUT_`，不暴露 SDK 头文件，使用方只依赖 OpenCV。
- **像素格式统一**：用 `MV_CC_ConvertPixelTypeEx` 把 Bayer / Mono / RGB 统一转成 `BGR8`，`read()` 拿到的 `cv::Mat` 一定是 `CV_8UC3`。
- **参数顺序**：手动写 `ExposureTime` / `Gain` 前必须先关掉 `ExposureAuto` / `GainAuto`，否则写入无效。

## 待实机验证

- [ ] 连接 hikrobot 相机，确认 `open()` 打印的参数与预期一致
- [ ] 确认 `flip_code: 2` 是否符合实际安装朝向（不合适就改 `configs/camera.yaml`）
- [ ] 确认装甲板检测框与角点绘制正常
