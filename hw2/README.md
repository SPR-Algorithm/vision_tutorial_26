# 作业二：相机类编写与YOLO目标检测

> **目标**：学会编写相机类（MyCamera），实现图像采集、显示和YOLO检测结果绘制

## 任务概述

这次作业需要线下到队里连接相机进行测试。
本作业包含两个主要任务：

### 任务一：编写相机类

- 参考 `example.cpp` 中的代码，完成 `MyCamera` 类的编写
- 实现相机初始化、图像采集和显示功能
- 相机类接口需要兼容 hikrobot 函数调用

### 任务二：YOLO检测结果绘制

- 根据 `main.cpp` 中的注释提示
- 完成对YOLO检测结果的可视化绘制
- 使用提供的工具函数完成绘制任务
---

## 前置知识

在开始作业前，请确保您具备以下知识基础：

- **OpenCV基础**：熟悉OpenCV的基本使用方法
- **C++类设计**：熟悉C++类的编写和封装方法
- **基础图像处理**：了解图像读取、显示和处理流程

---

## 环境配置

### 依赖库清单

| 库名称 | 版本要求 | 用途说明 |
|--------|----------|----------|
| `opencv` | 4.x | 图像处理和计算机视觉 |
| `C++` | C++17 | 编程语言标准 |
| `yaml-cpp` | - | 配置文件解析 |
| `eigen3` | - | 线性代数运算 |
| `fmt` | - | 格式化输出 |
| `spdlog` | - | 日志记录 |
| `openvino` | - | 深度学习推理 |

### 快速安装

```bash
# Ubuntu/Debian
sudo apt-get install libopencv-dev libyaml-cpp-dev libeigen3-dev libfmt-dev

# macOS (使用 Homebrew)
brew install opencv yaml-cpp eigen fmt

# 其他依赖请参考官方文档进行安装
```

---

## 重要提示

> **代码复用提醒**
>
> - **代码量较大**：本次作业涉及较多代码阅读，请耐心理解项目结构
> - **绘制工具**：使用 `tools/img_tools` 中的函数完成绘制，无需从零编写
> - **日志功能**：可以使用 `logger` 模块完成日志记录
> - **YOLO使用**：参考 `yolo.cpp` 了解YOLO模块的使用方法
> - **工具学习**：重点了解 `tools` 目录下各工具的用法

---

## 参考资料

### 官方文档

1. [OpenCV官方文档](https://docs.opencv.org/4.x/d1/dfb/intro.html)
   - *想快速完成作业？重点关注轮廓绘制相关内容*

2. [OpenCV中文文档](https://apachecn.github.io/opencv-doc-zh/#/)
   - *Python版本，但C++用法类似*

3. [C++类的封装与使用](https://cloud.tencent.com/developer/article/2557778)
   - *帮助理解面向对象编程概念*

### 项目结构导航

```text
homework/
├── main.cpp              # 主程序入口
├── io/
│   ├── example.cpp        # 相机使用示例
│   ├── my_camera.cpp      # 待完成：相机类实现
│   └── my_camera.hpp      # 相机类头文件
├── tasks/
│   ├── yolo.cpp          # YOLO实现参考
│   └── yolo.hpp
└── tools/
    ├── img_tools.cpp     # 图像工具函数
    ├── img_tools.hpp
    ├── logger.cpp        # 日志工具
    └── logger.hpp
```

---

## 开始作业

1. **阅读代码**：从 `example.cpp` 开始，理解相机调用流程
2. **实现类**：在 `my_camera.cpp` 中实现相机类
3. **绘制结果**：参考 `img_tools` 完成检测结果可视化
4. **测试验证**：运行程序验证功能正确性

<p  align="center"> <b>GOOD LUCK</b> </p>