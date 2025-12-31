#pragma once
#include <opencv2/opencv.hpp>
#include "light_bar.h"
#include <vector>

// 装甲板类型
enum ArmorType {
    SMALL_ARMOR,    // 小装甲板
    LARGE_ARMOR,    // 大装甲板
    UNKNOWN_ARMOR   // 未知类型
};

struct ArmorInfo {
    // PnP解算结果
    cv::Mat rotation_vector;    // 旋转向量
    cv::Mat translation_vector; // 平移向量
    cv::Mat rotation_matrix;    // 旋转矩阵
    cv::Vec3d euler_angles;     // 欧拉角
    cv::Vec4d quaternion;       // 四元数
    
    // 装甲板状态
    cv::Rect2f bounding_box;    // 边界框
    cv::Point2f center;         // 中心点
    ArmorType type;             // 装甲板类型
    double confidence;          // 置信度
};

class Armor {
public:
    Armor();
    Armor(const LightBar& left_bar, const LightBar& right_bar);
    ~Armor();
    
    // 设置/获取属性
    void setLeftBar(const LightBar& bar);
    void setRightBar(const LightBar& bar);
    LightBar getLeftBar() const;
    LightBar getRightBar() const;
    
    // 装甲板计算
    cv::Rect2f getBoundingBox() const;
    cv::Point2f getCenter() const;
    double getWidth() const;
    double getHeight() const;
    double getAspectRatio() const;
    ArmorType getType() const;
    
    // 装甲板有效性检查
    bool isValid() const;
    
    // 绘制装甲板
    void draw(cv::Mat& image, const cv::Scalar& color = cv::Scalar(0, 0, 255), int thickness = 2) const;
    
    // 更新PnP解算结果
    void updatePnPResult(const ArmorInfo& info);
    ArmorInfo getPnPResult() const;
    
private:
    LightBar left_bar_;         // 左灯条
    LightBar right_bar_;        // 右灯条
    ArmorInfo armor_info_;      // 装甲板信息
};