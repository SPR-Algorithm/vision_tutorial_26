#include "img_tool.hpp"
#include <algorithm>

namespace ImgTool {

cv::Mat preprocessImage(const cv::Mat& src, const std::string& color, 
                       const PreprocessParams& params) {
    if (src.empty()) {
        return cv::Mat();
    }

    cv::Mat hsv, mask, result;
    
    // 转换为HSV颜色空间
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    
    // 高斯模糊去噪
    cv::Mat blurred;
    cv::GaussianBlur(hsv, blurred, 
                     cv::Size(params.gaussianKernelSize, params.gaussianKernelSize), 0);
    
    // 根据颜色类型创建掩码
    if (color == "red") {
        // 红色在HSV空间中跨越0度，需要两个范围
        cv::Mat mask1, mask2;
        cv::inRange(blurred, params.lowerRed1, params.upperRed1, mask1);
        cv::inRange(blurred, params.lowerRed2, params.upperRed2, mask2);
        mask = mask1 | mask2;  // 合并两个掩码
    } else if (color == "blue") {
        cv::inRange(blurred, params.lowerBlue, params.upperBlue, mask);
    } else {
        // 默认使用红色
        cv::Mat mask1, mask2;
        cv::inRange(blurred, params.lowerRed1, params.upperRed1, mask1);
        cv::inRange(blurred, params.lowerRed2, params.upperRed2, mask2);
        mask = mask1 | mask2;
    }
    
    // 形态学操作：去除噪点，连接断开的区域
    // 使用较大的核来连接离散的点
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, 
        cv::Size(params.morphKernelSize, params.morphKernelSize));
    
    // 先进行闭运算：连接断开的区域（膨胀+腐蚀）
    cv::morphologyEx(mask, result, cv::MORPH_CLOSE, kernel);
    
    // 再进行开运算：去除小噪点（腐蚀+膨胀）
    cv::morphologyEx(result, result, cv::MORPH_OPEN, kernel);
    
    // 如果点仍然离散，可以再次进行闭运算来进一步连接
    cv::morphologyEx(result, result, cv::MORPH_CLOSE, kernel);
    
    return result;
}

std::vector<LightBar> extractLightBars(const cv::Mat& binary, 
                                       const PreprocessParams& params) {
    std::vector<LightBar> lightBars;
    
    if (binary.empty() || binary.channels() != 1) {
        return lightBars;
    }
    
    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // 遍历所有轮廓
    for (const auto& contour : contours) {
        // 计算轮廓面积
        double area = cv::contourArea(contour);
        
        // 面积筛选
        if (area < params.minArea || area > params.maxArea) {
            continue;
        }
        
        // 将轮廓拟合为旋转矩形
        cv::RotatedRect rect = cv::minAreaRect(contour);
        
        // 创建 LightBar 对象
        LightBar lightBar(rect);
        
        // 使用长宽比筛选
        float width = std::min(rect.size.width, rect.size.height);
        float height = std::max(rect.size.width, rect.size.height);
        float aspectRatio = (width > 0.0f) ? (height / width) : 0.0f;
        
        if (aspectRatio >= params.minAspectRatio && aspectRatio <= params.maxAspectRatio) {
            lightBars.push_back(lightBar);
        }
    }
    
    // 按面积从大到小排序
    std::sort(lightBars.begin(), lightBars.end(), 
        [](const LightBar& a, const LightBar& b) {
            return a.getArea() > b.getArea();
        });
    
    return lightBars;
}

std::vector<LightBar> detectLightBars(const cv::Mat& src, const std::string& color,
                                     const PreprocessParams& params) {
    // 预处理图像
    cv::Mat binary = preprocessImage(src, color, params);
    
    // 提取 LightBar
    return extractLightBars(binary, params);
}

void drawLightBars(cv::Mat& image, const std::vector<LightBar>& lightBars,
                  const cv::Scalar& color, int thickness) {
    for (const auto& lightBar : lightBars) {
        lightBar.draw(image, color, thickness);
    }
}

} // namespace ImgTool

