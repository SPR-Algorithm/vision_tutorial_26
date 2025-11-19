#ifndef LIGHTBAR_HPP
#define LIGHTBAR_HPP

#include "opencv2/opencv.hpp"
class LightBar {
public:
    LightBar(const cv::RotatedRect& rect);
    bool isValid(float minArea, float minAspectRatio) const;
    void draw(cv::Mat& image, const cv::Scalar& color, int thickness =2) const;
    ~LightBar();
    
    // Getter 方法
    float getArea() const { return area; }
    float getAspectRatio() const { return aspectRatio; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    cv::RotatedRect getRotatedRect() const { return rotatedRect; }
    
private:
    cv::RotatedRect rotatedRect;
    float width;
    float height;
    float area;
    float aspectRatio;
};
#endif // LIGHTBAR_HPP