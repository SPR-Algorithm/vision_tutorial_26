#include "TraditionalArmor.hpp"
#include <iostream>
#include <string>

int main(){
    std::string path = "/home/ljl/workspace_lecture2/part_03/img_01.jpg";
    cv::Mat originalImg = cv::imread(path);
    if (originalImg.empty()) {
        std::cerr << "无法加载原始图像: " << path << std::endl;
        return -1;
    }
    cv::Mat grayImg = convertToGray(path);
    if (grayImg.empty()) return -1;
    cv::Mat binaryImg;
    binaryImage(grayImg, binaryImg);
    cv::Mat contourImg;
    drawContours(binaryImg, contourImg);
    cv::Mat rotatedImg;
    getRotationMatrix(contourImg, rotatedImg);
    geometricFilterAndDraw(rotatedImg, originalImg);
    return 0;
}