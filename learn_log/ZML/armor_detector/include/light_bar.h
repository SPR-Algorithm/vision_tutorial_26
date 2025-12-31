#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class LightBar {
public:
    LightBar();
    LightBar(const cv::RotatedRect& rect);
    ~LightBar();
    
    // 设置/获取属性
    void setRect(const cv::RotatedRect& rect);
    cv::RotatedRect getRect() const;
    cv::Point2f getCenter() const;
    double getLength() const;
    double getWidth() const;
    double getAngle() const;
    double getAspectRatio() const;
    
    // 有效性检查
    bool isValid() const;
    
    // 绘制灯条
    void draw(cv::Mat& image, const cv::Scalar& color = cv::Scalar(0, 255, 0), int thickness = 2) const;
    
private:
    cv::RotatedRect rect_;      // 旋转矩形
    double length_;             // 长度
    double width_;              // 宽度
    double angle_;              // 角度
    double aspect_ratio_;       // 长宽比
};