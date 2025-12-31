#include "../include/armor.h"
#include <algorithm>
#include <cmath>

Armor::Armor() {}

Armor::Armor(const LightBar& left_bar, const LightBar& right_bar) 
    : left_bar_(left_bar), right_bar_(right_bar) {}

Armor::~Armor() {}

void Armor::setLeftBar(const LightBar& bar) {
    left_bar_ = bar;
}

void Armor::setRightBar(const LightBar& bar) {
    right_bar_ = bar;
}

LightBar Armor::getLeftBar() const {
    return left_bar_;
}

LightBar Armor::getRightBar() const {
    return right_bar_;
}

cv::Rect2f Armor::getBoundingBox() const {
    cv::RotatedRect left_rect = left_bar_.getRect();
    cv::RotatedRect right_rect = right_bar_.getRect();
    
    // 获取两个灯条的所有顶点
    cv::Point2f left_points[4], right_points[4];
    left_rect.points(left_points);
    right_rect.points(right_points);
    
    // 找到所有点的最小/最大坐标
    float min_x = std::min({left_points[0].x, left_points[1].x, left_points[2].x, left_points[3].x,
                           right_points[0].x, right_points[1].x, right_points[2].x, right_points[3].x});
    float max_x = std::max({left_points[0].x, left_points[1].x, left_points[2].x, left_points[3].x,
                           right_points[0].x, right_points[1].x, right_points[2].x, right_points[3].x});
    float min_y = std::min({left_points[0].y, left_points[1].y, left_points[2].y, left_points[3].y,
                           right_points[0].y, right_points[1].y, right_points[2].y, right_points[3].y});
    float max_y = std::max({left_points[0].y, left_points[1].y, left_points[2].y, left_points[3].y,
                           right_points[0].y, right_points[1].y, right_points[2].y, right_points[3].y});
    
    return cv::Rect2f(min_x, min_y, max_x - min_x, max_y - min_y);
}

cv::Point2f Armor::getCenter() const {
    cv::Point2f left_center = left_bar_.getCenter();
    cv::Point2f right_center = right_bar_.getCenter();
    return cv::Point2f((left_center.x + right_center.x) / 2, 
                       (left_center.y + right_center.y) / 2);
}

double Armor::getWidth() const {
    cv::Point2f left_center = left_bar_.getCenter();
    cv::Point2f right_center = right_bar_.getCenter();
    return cv::norm(left_center - right_center);
}

double Armor::getHeight() const {
    return std::max(left_bar_.getLength(), right_bar_.getLength());
}

double Armor::getAspectRatio() const {
    double width = getWidth();
    double height = getHeight();
    return (height > 0) ? width / height : 0;
}

ArmorType Armor::getType() const {
    double aspect_ratio = getAspectRatio();
    if (aspect_ratio < 2.5) {
        return SMALL_ARMOR;
    } else if (aspect_ratio >= 2.5 && aspect_ratio < 4.5) {
        return LARGE_ARMOR;
    }
    return UNKNOWN_ARMOR;
}

bool Armor::isValid() const {
    return left_bar_.isValid() && right_bar_.isValid();
}

void Armor::draw(cv::Mat& image, const cv::Scalar& color, int thickness) const {
    // 绘制左右灯条
    left_bar_.draw(image, cv::Scalar(0, 255, 0), thickness);
    right_bar_.draw(image, cv::Scalar(0, 255, 0), thickness);
    
    // 绘制装甲板边界框
    cv::Rect2f bbox = getBoundingBox();
    cv::rectangle(image, bbox, color, thickness);
    
    // 绘制中心点
    cv::Point2f center = getCenter();
    cv::circle(image, center, 4, cv::Scalar(0, 0, 255), -1);
    
    // 绘制连接线
    cv::line(image, left_bar_.getCenter(), right_bar_.getCenter(), 
             cv::Scalar(255, 255, 0), thickness);
}

void Armor::updatePnPResult(const ArmorInfo& info) {
    armor_info_ = info;
}

ArmorInfo Armor::getPnPResult() const {
    return armor_info_;
}