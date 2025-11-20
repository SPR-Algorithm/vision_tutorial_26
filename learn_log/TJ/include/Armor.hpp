#ifndef LIGHTBAR_HPP
#define LIGHTBAR_HPP

#include "opencv2/opencv.hpp"



class Armor
{
public:
    Armor();

    ~Armor();
private:
    // Armor class implementation
    

};

struct LightBar {
    cv::RotatedRect rotatedRect; // 旋转矩形表示灯条
    float width;                 // 灯条宽度
    float height;                // 灯条高度
    float area;                  // 灯条面积
    float aspectRatio;           // 长宽比

    // 构造函数
    LightBar(const cv::RotatedRect& rect);

    // 方法：验证灯条是否符合预设条件
    bool isValid(float minArea, float minAspectRatio) const;

    // 方法：绘制灯条轮廓
    void draw(cv::Mat& image, const cv::Scalar& color, int thickness = 2) const;
    // 析构函数
    ~LightBar();
};
#endif // LIGHTBAR_HPP