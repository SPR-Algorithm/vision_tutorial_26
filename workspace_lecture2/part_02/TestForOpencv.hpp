#ifndef TESTFOROPENCV_HPP
#define TESTFOROPENCV_HPP

#include <opencv2/opencv.hpp>
#include <string>

void convertToGray(const std::string& imagePath) {
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        std::cerr << "无法加载图像: " << imagePath << std::endl;
        return;
    }

    // 按照RGB通道拆分
    std::vector<cv::Mat> channels;
    cv::split(img, channels); // channels[0]=B, channels[1]=G, channels[2]=R

    // 缩小每个通道
    for (int i = 0; i < 3; ++i) {
        cv::Mat resizedImg;
        cv::resize(channels[i], resizedImg, cv::Size(), 0.2, 0.2); // 缩小5倍
        cv::imshow(std::string("Gray Channel ") + std::to_string(i), resizedImg);
    }

    cv::waitKey(0);
}

#endif