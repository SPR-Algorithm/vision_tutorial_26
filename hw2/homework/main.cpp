#include <fmt/core.h>

#include <algorithm>
#include <iostream>

#include "io/my_camera.hpp"
#include "opencv2/opencv.hpp"
#include "tasks/yolo.hpp"
#include "tools/img_tools.hpp"

using namespace auto_aim;

int main()
{
  // 初始化相机、yolo类
  io::MyCamera camera("configs/camera.yaml");
  if (!camera.open()) {
    std::cerr << "[main] Camera open failed!" << std::endl;
    return -1;
  }

  YOLO yolo("configs/yolo.yaml", true);

  cv::Mat img;
  int frame_count = 0;

  while (1) {
    // 读取相机图像
    if (!camera.read(img)) {
      continue;
    }

    // 调用yolo识别装甲板
    auto armors = yolo.detect(img, frame_count);

    // 绘制检测结果
    for (const auto & armor : armors) {
      // 1. 检测框
      cv::rectangle(img, armor.box, {0, 255, 0}, 2);

      // 2. 装甲板四个角点（左上、右上、右下、左下）
      tools::draw_points(img, armor.points, {0, 255, 0}, 2);

      // 3. 装甲板中心点
      cv::Point center(
        static_cast<int>(armor.center.x), static_cast<int>(armor.center.y));
      tools::draw_point(img, center, {255, 0, 0}, 4);

      // 4. 置信度 + 颜色 + 编号 + 大小
      auto info = fmt::format(
        "{:.2f} {} {} {}", armor.confidence, COLORS[armor.color], ARMOR_NAMES[armor.name],
        ARMOR_TYPES[armor.type]);
      cv::Point text_pos(armor.box.x, std::max(armor.box.y - 5, 20));
      tools::draw_text(img, info, text_pos, {0, 255, 255}, 0.5, 2);
    }

    // 显示图像
    tools::draw_text(img, fmt::format("[{}]", frame_count), {10, 30}, {255, 255, 255}, 0.8, 2);
    cv::resize(img, img, cv::Size(640, 480));
    cv::imshow("img", img);

    frame_count++;
    if (cv::waitKey(1) == 'q') {
      break;
    }
  }

  return 0;
}
