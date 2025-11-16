本次作业：

1.在虚拟机上成功部署ros2

2.按照官方文档构建自己的工作空间和功能包（可以以自己名字命名 例如：zengke_ws）

3.自行阅读官方文档本节相关内容[`ROS2原文文档`](https://docs.ros.org/en/kilted/index.html)，使用C++编写两个节点，一个负责发出两个随机数a，b作为直角边长度的数据到话题 /rightangledside ，一个话题负责订阅直角边数据并计算出斜边长度并发送到话题 /obliqueside（所有数据类型都使用float浮点数）

（建议使用本次内容中的发布订阅作为通讯方式）




# 使用方法（Usage）

本功能包 **pubsub_trans** 包含两个节点：

* [`publisher_celsius_function.cpp`](src/publisher_celsius_function.cpp)
* [`subscriber_fahrenheit_function.cpp`](src/subscriber_fahrenheit_function.cpp)

分别用于发布摄氏温度数据至 `/celsius` 话题，以及订阅 `/celsius` 并将摄氏度转换为华氏度后输出到终端。

---

## 1. 将功能包放入工作空间

将 `pubsub_trans` 目录放入你的 ROS2 工作空间的 `src/` 目录中，例如：

```
your_ws/
└── src/
    └── pubsub_trans/
```

若你的工作空间名为 `zengke_ws`，目录结构如下：

```
zengke_ws/
└── src/
    └── pubsub_trans/
```

---

## 2. 在工作空间根目录编译

进入工作空间根目录并执行：

```bash
cd ~/zengke_ws
colcon build
```

成功后将生成 `build/`、`install/`、`log/` 目录。

---

## 3. 设置环境变量

每次打开新终端运行前都需要执行：

```bash
source install/setup.bash
```

或（使用 zsh）：

```bash
source install/setup.zsh
```

---

## 4. 运行摄氏温度发布节点

在设置好环境变量的终端中执行：

```bash
ros2 run pubsub_trans celsius_pub
```

此节点会持续向 `/celsius` 发布浮点数格式的摄氏温度。

---

## 5. 在第二个终端运行华氏温度订阅节点

打开另一个终端，进入工作空间并加载环境变量：

```bash
cd ~/zengke_ws
source install/setup.bash
```

运行订阅节点：

```bash
ros2 run pubsub_trans fahrenheit_sub
```

该节点将订阅 `/celsius` 并在终端打印转换后的华氏温度。

---

## 6. 可选：查看话题

列出当前话题：

```bash
ros2 topic list
```

查看 `/celsius` 内容：

```bash
ros2 topic echo /celsius
```

---








