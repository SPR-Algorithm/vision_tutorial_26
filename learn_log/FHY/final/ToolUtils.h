#ifndef TOOL_UTILS_H
#define TOOL_UTILS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace cv;
using namespace std;

class ToolUtils
{
public:
    // 角度修正
    static float correctAngle(const RotatedRect& r_rect);
    // 两点距离计算
    static float calcDistance(const Point2f& p1, const Point2f& p2);
    // 顶点Y轴排序
    static void sortVerticesByY(Point2f pts[4], Point2f& top, Point2f& bottom);
};

#endif // TOOL_UTILS_H