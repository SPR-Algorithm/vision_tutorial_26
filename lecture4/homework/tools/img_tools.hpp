#ifndef TOOLS__IMG_TOOLS_HPP
#define TOOLS__IMG_TOOLS_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace tools
{
void draw_point(
  cv::Mat & img, const cv::Point & point, const cv::Scalar & color = {0, 0, 255}, int radius = 3);
}  // namespace tools

#endif  // TOOLS__IMG_TOOLS_HPP