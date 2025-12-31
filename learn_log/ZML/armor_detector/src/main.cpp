#include "../include/armor_detector.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    // 默认视频路径
    std::string video_path = "/home/a/final/video.avi";
    
    // 如果提供了命令行参数，使用提供的视频路径
    if (argc > 1) {
        video_path = argv[1];
        std::cout << "使用视频文件: " << video_path << std::endl;
    } else {
        std::cout << "使用默认视频文件: " << video_path << std::endl;
    }
    
    ArmorDetector::Params params;
    
    // 基本参数（可根据实际情况调整）
    params.min_light_area = 100.0;
    params.min_light_aspect_ratio = 1.5;
    params.max_light_aspect_ratio = 8.0;
    params.min_armor_area = 500.0;
    
    // 颜色检测参数调整（根据实际情况调整）
    params.red_lower1 = cv::Scalar(0, 100, 100);      // 提高饱和度阈值
    params.red_upper1 = cv::Scalar(10, 255, 255);
    params.red_lower2 = cv::Scalar(160, 100, 100);
    params.red_upper2 = cv::Scalar(180, 255, 255);
    
    // 不指定相机参数文件（避免文件不存在错误）
    params.camera_params_path = "/home/a/log/armor_detector/resources/camera_params.yml";
    
    std::cout << "\n========== 装甲板检测器 ==========" << std::endl;
    std::cout << "视频文件: " << video_path << std::endl;
    std::cout << "=================================\n" << std::endl;
    
    std::cout << "创建装甲板检测器..." << std::endl;
    
    try {
        ArmorDetector detector(params);
        
        std::cout << "开始处理视频..." << std::endl;
        std::cout << "操作提示:" << std::endl;
        std::cout << "  ESC键 - 退出程序" << std::endl;
        std::cout << "  空格键 - 暂停/继续" << std::endl;
        
        detector.processVideo(video_path);
        
    } catch (const cv::Exception& e) {
        std::cerr << "\nOpenCV错误: " << e.what() << std::endl;
        std::cerr << "请检查视频文件格式和OpenCV安装" << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "\n程序异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n未知错误" << std::endl;
        return 1;
    }
    
    std::cout << "\n处理完成" << std::endl;
    
    return 0;
}