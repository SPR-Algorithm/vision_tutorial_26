#include "../include/armor_detector.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>

ArmorDetector::ArmorDetector() {
    init3DPoints();
    pnp_solver_ = std::make_shared<PnPSolver>();
}

ArmorDetector::ArmorDetector(const Params& params) : params_(params) {
    init3DPoints();
    pnp_solver_ = std::make_shared<PnPSolver>();
    
    // 尝试加载相机参数，但如果失败就继续运行（仅影响PnP解算）
    if (!params_.camera_params_path.empty()) {
        try {
            cv::FileStorage fs(params_.camera_params_path, cv::FileStorage::READ);
            if (fs.isOpened()) {
                cv::Mat camera_matrix, dist_coeffs;
                fs["camera_matrix"] >> camera_matrix;
                fs["distortion_coefficients"] >> dist_coeffs;
                
                if (!camera_matrix.empty() && !dist_coeffs.empty()) {
                    pnp_solver_->setCameraParams(camera_matrix, dist_coeffs);
                    std::cout << "相机参数加载成功" << std::endl;
                } else {
                    std::cout << "警告：相机参数文件内容为空，使用默认参数" << std::endl;
                    setDefaultCameraParams();
                }
                fs.release();
            } else {
                std::cout << "警告：无法打开相机参数文件 '" << params_.camera_params_path 
                         << "'，使用默认参数" << std::endl;
                setDefaultCameraParams();
            }
        } catch (const cv::Exception& e) {
            std::cerr << "警告：加载相机参数时出错: " << e.what() << std::endl;
            std::cerr << "使用默认相机参数" << std::endl;
            setDefaultCameraParams();
        }
    } else {
        std::cout << "警告：未指定相机参数文件路径，使用默认参数" << std::endl;
        setDefaultCameraParams();
    }
}

ArmorDetector::~ArmorDetector() = default;

void ArmorDetector::setDefaultCameraParams() {
    // 设置默认相机参数（假设640x480图像）
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 
        800.0, 0.0, 320.0,
        0.0, 800.0, 240.0,
        0.0, 0.0, 1.0);
    
    cv::Mat dist_coeffs = cv::Mat::zeros(5, 1, CV_64F);
    
    pnp_solver_->setCameraParams(camera_matrix, dist_coeffs);
}

cv::Mat ArmorDetector::preprocess(const cv::Mat& image) {
    cv::Mat result;
    cv::GaussianBlur(image, result, 
                     cv::Size(params_.gaussian_kernel_size, params_.gaussian_kernel_size),
                     params_.gaussian_sigma);
    return result;
}

cv::Mat ArmorDetector::colorSegmentation(const cv::Mat& image) {
    cv::Mat hsv, mask1, mask2, mask;
    
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    
    cv::inRange(hsv, params_.red_lower1, params_.red_upper1, mask1);
    cv::inRange(hsv, params_.red_lower2, params_.red_upper2, mask2);
    
    mask = mask1 | mask2;
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                     cv::Size(params_.morph_kernel_size, params_.morph_kernel_size));
    
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    
    return mask;
}

std::vector<LightBar> ArmorDetector::detectLightBars(const cv::Mat& binary_image) {
    std::vector<LightBar> light_bars;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(binary_image, contours, hierarchy, 
                     cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area < params_.min_light_area || contour.size() < 5) {
            continue;
        }
        
        cv::RotatedRect rect = cv::fitEllipse(contour);
        LightBar light_bar(rect);
        
        double aspect_ratio = light_bar.getAspectRatio();
        double angle = light_bar.getAngle();
        
        if (aspect_ratio >= params_.min_light_aspect_ratio &&
            aspect_ratio <= params_.max_light_aspect_ratio &&
            angle >= params_.min_light_angle &&
            angle <= params_.max_light_angle) {
            light_bars.push_back(light_bar);
        }
    }
    
    return light_bars;
}

std::vector<Armor> ArmorDetector::matchArmors(const std::vector<LightBar>& light_bars) {
    std::vector<Armor> armors;
    
    if (light_bars.size() < 2) {
        return armors;
    }
    
    for (size_t i = 0; i < light_bars.size(); i++) {
        for (size_t j = i + 1; j < light_bars.size(); j++) {
            const LightBar& bar1 = light_bars[i];
            const LightBar& bar2 = light_bars[j];
            
            double center_diff_y = std::abs(bar1.getCenter().y - bar2.getCenter().y);
            double angle_diff = std::abs(bar1.getAngle() - bar2.getAngle());
            
            if (center_diff_y < params_.max_light_center_diff &&
                angle_diff < params_.max_light_angle_diff) {
                
                LightBar left_bar, right_bar;
                if (bar1.getCenter().x < bar2.getCenter().x) {
                    left_bar = bar1;
                    right_bar = bar2;
                } else {
                    left_bar = bar2;
                    right_bar = bar1;
                }
                
                Armor armor(left_bar, right_bar);
                double armor_area = armor.getBoundingBox().area();
                
                if (armor_area >= params_.min_armor_area) {
                    armors.push_back(armor);
                }
            }
        }
    }
    
    return armors;
}

void ArmorDetector::init3DPoints() {
    // 小装甲板3D点（单位：mm）
    small_armor_points_.push_back(cv::Point3f(-67.5, -27.5, 0));
    small_armor_points_.push_back(cv::Point3f(67.5, -27.5, 0));
    small_armor_points_.push_back(cv::Point3f(67.5, 27.5, 0));
    small_armor_points_.push_back(cv::Point3f(-67.5, 27.5, 0));
    
    // 大装甲板3D点（单位：mm）
    large_armor_points_.push_back(cv::Point3f(-112.5, -27.5, 0));
    large_armor_points_.push_back(cv::Point3f(112.5, -27.5, 0));
    large_armor_points_.push_back(cv::Point3f(112.5, 27.5, 0));
    large_armor_points_.push_back(cv::Point3f(-112.5, 27.5, 0));
}

std::vector<Armor> ArmorDetector::detect(const cv::Mat& image) {
    std::vector<Armor> armors;
    
    // 创建调试图像
    debug_image_ = image.clone();
    
    // 1. 图像预处理
    cv::Mat processed = preprocess(image);
    
    // 2. 颜色分割
    cv::Mat binary = colorSegmentation(processed);
    
    // 3. 检测灯条
    std::vector<LightBar> light_bars = detectLightBars(binary);
    
    // 4. 匹配装甲板
    armors = matchArmors(light_bars);
    
    // 绘制检测结果
    for (auto& armor : armors) {
        armor.draw(debug_image_);
    }
    
    return armors;
}

void ArmorDetector::processVideo(const std::string& video_path) {
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频文件: " << video_path << std::endl;
        return;
    }
    
    cv::Mat frame;
    int frame_count = 0;
    
    while (cap.read(frame)) {
        if (frame.empty()) break;
        
        frame_count++;
        std::cout << "处理第 " << frame_count << " 帧" << std::endl;
        
        // 检测装甲板
        std::vector<Armor> armors = detect(frame);
        
        // 显示结果
        cv::imshow("Armor Detection", debug_image_);
        
        std::cout << "检测到装甲板数量: " << armors.size() << std::endl;
        
        // 按ESC退出
        int key = cv::waitKey(30);
        if (key == 27) {
            std::cout << "用户中断处理" << std::endl;
            break;
        } else if (key == ' ') {  // 空格键暂停
            std::cout << "暂停，按任意键继续..." << std::endl;
            cv::waitKey(0);
        }
    }
    
    cap.release();
    cv::destroyAllWindows();
    std::cout << "视频处理完成，共处理 " << frame_count << " 帧" << std::endl;
}