#ifndef IMG_TOOL_HPP
#define IMG_TOOL_HPP

#include "opencv2/opencv.hpp"
#include "lightbar.hpp"
#include <vector>

namespace ImgTool {
    /**
     * @brief 图像预处理参数结构体
     */
    struct PreprocessParams {
        // 颜色阈值参数（HSV空间）
        cv::Scalar lowerRed1 = cv::Scalar(0, 50, 50);      // 红色下界1（0-10）
        cv::Scalar upperRed1 = cv::Scalar(10, 255, 255);   // 红色上界1
        cv::Scalar lowerRed2 = cv::Scalar(170, 50, 50);    // 红色下界2（170-180）
        cv::Scalar upperRed2 = cv::Scalar(180, 255, 255);  // 红色上界2
        cv::Scalar lowerBlue = cv::Scalar(100, 50, 50);    // 蓝色下界
        cv::Scalar upperBlue = cv::Scalar(130, 255, 255);  // 蓝色上界
        
        // 形态学操作参数
        int morphKernelSize = 10;      // 形态学核大小（增大以连接离散的点）
        int gaussianKernelSize = 5;    // 高斯模糊核大小
        
        // LightBar筛选参数
        float minArea = 10.0f;         // 最小面积
        float maxArea = 500.0f;      // 最大面积
        float minAspectRatio = 4.0f;    // 最小长宽比
        float maxAspectRatio = 20.0f;   // 最大长宽比
    };

    /**
     * @brief 图像预处理：提取特定颜色的区域
     * @param src 输入图像（BGR格式）
     * @param color 要提取的颜色："red" 或 "blue"
     * @param params 预处理参数
     * @return 二值化后的图像
     */
    cv::Mat preprocessImage(const cv::Mat& src, const std::string& color = "red", 
                           const PreprocessParams& params = PreprocessParams());

    /**
     * @brief 从预处理后的二值图像中提取 LightBar
     * @param binary 二值化图像
     * @param params 筛选参数
     * @return 提取到的 LightBar 向量
     */
    std::vector<LightBar> extractLightBars(const cv::Mat& binary, 
                                           const PreprocessParams& params = PreprocessParams());

    /**
     * @brief 从原始图像中直接提取 LightBar（包含预处理步骤）
     * @param src 输入图像（BGR格式）
     * @param color 要提取的颜色："red" 或 "blue"
     * @param params 预处理和筛选参数
     * @return 提取到的 LightBar 向量
     */
    std::vector<LightBar> detectLightBars(const cv::Mat& src, const std::string& color = "blue",
                                         const PreprocessParams& params = PreprocessParams());

    /**
     * @brief 在图像上绘制所有检测到的 LightBar
     * @param image 要绘制的图像
     * @param lightBars LightBar 向量
     * @param color 绘制颜色
     * @param thickness 线条粗细
     */
    void drawLightBars(cv::Mat& image, const std::vector<LightBar>& lightBars,
                      const cv::Scalar& color = cv::Scalar(0, 255, 0), int thickness = 2);
}

#endif // IMG_TOOL_HPP

