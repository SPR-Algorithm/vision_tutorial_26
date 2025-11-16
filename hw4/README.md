本次作业：

1.在虚拟机上成功部署ros2

2.按照官方文档构建自己的工作空间和功能包（可以以自己名字命名 例如：zengke_ws）

3.自行阅读官方文档本节相关内容，使用C++编写两个节点，一个负责发出两个随机数a，b作为直角边长度的数据到话题 /rightangledside ，一个话题负责订阅直角边数据并计算出斜边长度并发送到话题 /obliqueside（所有数据类型都使用float浮点数）

（建议使用本次内容中的发布订阅作为通讯方式）


  pubsub_trans 软件包包含两个节点 [publisher_celsius_function.cpp](https://github.com/SPR-Algorithm/vision_tutorial_26/blob/main/hw4/pubsub_trans/src/publisher_celsius_function.cpp)  和 [subscriber_fahrenheit_function.cpp](https://github.com/SPR-Algorithm/vision_tutorial_26/blob/main/hw4/pubsub_trans/src/subscriber_fahrenheit_function.cpp)   ，分别实现发布摄氏度的数据到 `/celsius` 话题，以及通过订阅 `/celsius` 话题来读取摄氏度的数据并转换成华氏温度道德数据发布到终端上  

使用方法





