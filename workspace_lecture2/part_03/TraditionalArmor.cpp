#include "TraditionalArmor.hpp"
#include <iostream>

// step1 获取灰度图
cv::Mat convertToGray(const std::string& imagePath){
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        std::cerr << "无法加载图像: " << imagePath << std::endl;
        return cv::Mat();
    }
    cv::Mat grayImg;
    cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
    cv::imshow("Gray Image", grayImg);
    cv::waitKey(0);
    return grayImg;
}

// step2 进行二值化
cv::Mat binaryImage(const cv::Mat& grayImg, cv::Mat& binaryImg){
    cv::threshold(grayImg, binaryImg, 128, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Image", binaryImg);
    cv::waitKey(0);
    return binaryImg;
}

// step3 描绘轮廓
cv::Mat drawContours(const cv::Mat& binaryImg, cv::Mat& contourImg){
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    contourImg = cv::Mat::zeros(binaryImg.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::drawContours(contourImg, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);
    }
    cv::imshow("Contours", contourImg);
    cv::waitKey(0);
    return contourImg;
}

// step4 获得框出轮廓的最小矩形
void getRotationMatrix(const cv::Mat& contourImg, cv::Mat& rotatedImg){
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat gray;
    if (contourImg.channels() == 3) {
        cv::cvtColor(contourImg, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = contourImg.clone();
    }
    cv::findContours(gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    rotatedImg = contourImg.clone();
    for (const auto& contour : contours) {
        cv::RotatedRect minRect = cv::minAreaRect(contour);
        cv::Point2f vertices[4];
        minRect.points(vertices);
        for (int j = 0; j < 4; j++) {
            cv::line(rotatedImg, vertices[j], vertices[(j + 1) % 4], cv::Scalar(0, 0, 255), 2);
        }
    }
    cv::imshow("Rotated Rectangles", rotatedImg);
    cv::waitKey(0);
}

// step5 进行几何筛选，把y轴长度占比大的矩形画在原图上
void geometricFilterAndDraw(const cv::Mat& rotatedImg, const cv::Mat& originalImg){
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat gray;
    if (rotatedImg.channels() == 3) {
        cv::cvtColor(rotatedImg, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = rotatedImg.clone();
    }
    cv::findContours(gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat resultImg = originalImg.clone();
    for (const auto& contour : contours) {
        cv::Rect bbox = cv::boundingRect(contour);
        int x_length = bbox.width;
        int y_length = bbox.height;
        if (y_length > x_length * 1.5) { // y轴方向长度大于x轴方向长度的1.5倍
            cv::rectangle(resultImg, bbox, cv::Scalar(255, 0, 0), 2);
        }
    }
    cv::imshow("Filtered Rectangles", resultImg);
    cv::waitKey(0);
}