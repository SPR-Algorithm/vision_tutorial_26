#ifndef TRADITIONALARMOR_HPP
#define TRADITIONALARMOR_HPP
#include <opencv2/opencv.hpp>
#include <string>

cv::Mat convertToGray(const std::string& imagePath);

cv::Mat binaryImage(const cv::Mat& grayImg, cv::Mat& binaryImg);

cv::Mat drawContours(const cv::Mat& binaryImg, cv::Mat& contourImg);

void getRotationMatrix(const cv::Mat& contourImg, cv::Mat& rotatedImg);

void geometricFilterAndDraw(const cv::Mat& rotatedImg, const cv::Mat& originalImg);

#endif // TRADITIONALARMOR_HPP