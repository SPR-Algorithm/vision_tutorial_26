#include "lightbar.hpp"

LightBar::LightBar(const cv::RotatedRect& rect): rotatedRect(rect) {
    width = std::min(rotatedRect.size.width, rotatedRect.size.height);
    height = std::max(rotatedRect.size.width, rotatedRect.size.height);
    area = rotatedRect.size.area();
    // 防止除零错误
    aspectRatio = (width > 0.0f) ? (height / width) : 0.0f;
}

bool LightBar::isValid(float minArea, float minAspectRatio) const {
    return (area >= minArea) && (aspectRatio >= minAspectRatio);
}

void LightBar::draw(cv::Mat& image, const cv::Scalar& color, int thickness) const {
    cv::Point2f points[4];
    rotatedRect.points(points);
    for (int i = 0; i < 4; i++) {
        cv::line(image, points[i], points[(i + 1) % 4], color, thickness);
    }
}

LightBar::~LightBar() {
    // 析构函数：清理资源（当前类没有动态分配的内存，无需特殊处理）
}