#pragma once
#include "armor.h"
#include "pnp_solver.h"
#include <memory>
#include <vector>

class ArmorDetector {
public:
    struct Params {
        // 预处理参数
        int gaussian_kernel_size = 5;
        double gaussian_sigma = 1.0;
        int morph_kernel_size = 3;
        
        // 颜色检测参数（红色）
        cv::Scalar red_lower1 = cv::Scalar(0, 50, 50);
        cv::Scalar red_upper1 = cv::Scalar(10, 255, 255);
        cv::Scalar red_lower2 = cv::Scalar(160, 50, 50);
        cv::Scalar red_upper2 = cv::Scalar(180, 255, 255);
        
        // 灯条筛选参数
        double min_light_area = 50.0;
        double min_light_aspect_ratio = 1.5;
        double max_light_aspect_ratio = 10.0;
        double min_light_angle = -30.0;
        double max_light_angle = 30.0;
        
        // 装甲板筛选参数
        double min_armor_area = 100.0;
        double max_light_center_diff = 50.0;
        double max_light_angle_diff = 15.0;
        
        // PnP参数
        std::string camera_params_path = "";
    };
    
    ArmorDetector();
    explicit ArmorDetector(const Params& params);
    ~ArmorDetector();
    
    void processVideo(const std::string& video_path);
    std::vector<Armor> detect(const cv::Mat& image);
    
private:
    void setDefaultCameraParams();  // 添加这个声明
    cv::Mat preprocess(const cv::Mat& image);
    cv::Mat colorSegmentation(const cv::Mat& image);
    std::vector<LightBar> detectLightBars(const cv::Mat& binary_image);
    std::vector<Armor> matchArmors(const std::vector<LightBar>& light_bars);
    void init3DPoints();
    
    Params params_;
    std::shared_ptr<PnPSolver> pnp_solver_;
    cv::Mat debug_image_;
    std::vector<cv::Point3f> small_armor_points_;
    std::vector<cv::Point3f> large_armor_points_;
};