#include "lightbar.hpp"
#include "img_tool.hpp"
#include <iostream>
#include <string>

int main()
{
    // 使用相对路径，更灵活
    std::string videoPath = "/Users/kyoko/Team_project/vision_tutorial_26/learn_log/TJ/src/demo.avi";
    cv::VideoCapture cap(videoPath);
    
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频！请检查路径: " << videoPath << std::endl;
        return -1;
    }
    
    // 配置预处理参数
    ImgTool::PreprocessParams params;
    params.minArea = 100.0f;           // 最小面积
    params.maxArea = 10000.0f;         // 最大面积
    params.minAspectRatio = 2.0f;      // 最小长宽比
    params.maxAspectRatio = 10.0f;     // 最大长宽比
    
    cv::Mat frame, binary;
    std::string colorMode = "red";  // 可以选择 "red" 或 "blue"
    
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cout << "视频处理完毕" << std::endl;
            break;
        }
        
        // 使用图像预处理和 LightBar 检测
        std::vector<LightBar> lightBars = ImgTool::detectLightBars(frame, colorMode, params);
        
        // 使用 LightBar 类的 draw() 方法绘制检测到的灯条
        for (const auto& lightBar : lightBars) {
            lightBar.draw(frame, cv::Scalar(0, 255, 0), 2);  // 绿色框，厚度为2
        }
        
        // 显示检测结果数量
        std::string info = "Detected: " + std::to_string(lightBars.size()) + " LightBars";
        cv::putText(frame, info, cv::Point(10, 30), 
                   cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);
        
        // 可选：显示预处理后的二值图像
        binary = ImgTool::preprocessImage(frame, colorMode, params);
        cv::imshow("Binary", binary);
        
        cv::imshow("LightBar Detection", frame);
        
        int key = cv::waitKey(30) & 0xFF;
        if (key == 27) {  // ESC键退出
            break;
        } else if (key == 'r' || key == 'R') {  // 按R键切换红色
            colorMode = "red";
            std::cout << "切换到红色检测模式" << std::endl;
        } else if (key == 'b' || key == 'B') {  // 按B键切换蓝色
            colorMode = "blue";
            std::cout << "切换到蓝色检测模式" << std::endl;
        }
    }
    
    cap.release();
    cv::destroyAllWindows();
    return 0;
}