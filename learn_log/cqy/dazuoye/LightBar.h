#ifndef LIGHT_BAR_H
#define LIGHT_BAR_H

#include <opencv2/opencv.hpp>
#include "ToolUtils.h"
#include "ArmorConfig.h"

using namespace cv;
using namespace std;

class LightBar
{
private:
    RotatedRect m_rect;       // 灯条旋转矩形
    float m_area;             // 灯条轮廓面积
    float m_width;            // 灯条短边
    float m_height;           // 灯条长边
    float m_ratio;            // 长宽比
    float m_angleDeviation;   // 角度偏差值

public:
    // 构造函数：传入轮廓，自动计算所有属性
    LightBar(const vector<Point>& contour);
    // 灯条有效性校验
    bool isValid() const;
    // 获取灯条旋转矩形
    RotatedRect getRect() const { return m_rect; }
    // 获取灯条长边长度
    float getHeight() const { return m_height; }
    // 获取灯条中心坐标
    Point2f getCenter() const { return m_rect.center; }
};

#endif // LIGHT_BAR_H