#include "TestForOpencv.hpp"
#include <iostream>

// void convertToGray(const std::string& imagePath) {
//     cv::Mat img = cv::imread(imagePath);
//     if (img.empty()) {
//         std::cerr << "无法加载图像: " << imagePath << std::endl;
//         return;
//     }
//     cv::Mat grayImg;
//     cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
//     cv::imshow("Gray Image", grayImg);
//     cv::waitKey(0);
// }

int main() {
    std::string path = "/home/ljl/workspace_lecture2/part_02/img_01.jpg";
    convertToGray(path);
    return 0;
}

