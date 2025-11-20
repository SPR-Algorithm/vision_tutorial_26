#include "Armor.hpp"

Armor::Armor()
{
    // Armor class constructor implementation
}

Armor::~Armor()
{
    // Armor class destructor implementation
}

LightBar::LightBar(const cv::RotatedRect& rect)
    : rotatedRect(rect)
{
    width = rect.size.width;
    height = rect.size.height;
    area = width * height;
    aspectRatio = height > 0 ? width / height : 0;
}

bool LightBar::isValid(float minArea, float minAspectRatio) const
{
    return area >= minArea && aspectRatio >= minAspectRatio;
}

void LightBar::draw(cv::Mat& image, const cv::Scalar& color, int thickness) const
{
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);
    for (int i = 0; i < 4; i++) {
        cv::line(image, vertices[i], vertices[(i + 1) % 4], color, thickness);
    }
}
LightBar::~LightBar()
{
    // LightBar class destructor implementation
}