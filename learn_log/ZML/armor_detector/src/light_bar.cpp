#include "../include/light_bar.h"
#include <cmath>

LightBar::LightBar() : length_(0), width_(0), angle_(0), aspect_ratio_(0) {}

LightBar::LightBar(const cv::RotatedRect& rect) {
    setRect(rect);
}

LightBar::~LightBar() {}

void LightBar::setRect(const cv::RotatedRect& rect) {
    rect_ = rect;
    length_ = std::max(rect.size.width, rect.size.height);
    width_ = std::min(rect.size.width, rect.size.height);
    angle_ = rect.angle;
    aspect_ratio_ = (width_ > 0) ? length_ / width_ : 0;
}

cv::RotatedRect LightBar::getRect() const {
    return rect_;
}

cv::Point2f LightBar::getCenter() const {
    return rect_.center;
}

double LightBar::getLength() const {
    return length_;
}

double LightBar::getWidth() const {
    return width_;
}

double LightBar::getAngle() const {
    return angle_;
}

double LightBar::getAspectRatio() const {
    return aspect_ratio_;
}

bool LightBar::isValid() const {
    return length_ > 0 && width_ > 0 && aspect_ratio_ > 0;
}

void LightBar::draw(cv::Mat& image, const cv::Scalar& color, int thickness) const {
    cv::Point2f vertices[4];
    rect_.points(vertices);
    for (int i = 0; i < 4; i++) {
        cv::line(image, vertices[i], vertices[(i + 1) % 4], color, thickness);
    }
    // 绘制中心点
    cv::circle(image, rect_.center, 3, cv::Scalar(255, 0, 0), -1);
}