#include "../include/pnp_solver.h"
#include <iostream>
#include <cmath>

// 内部辅助函数声明
namespace {
    cv::Vec3d rotationMatrixToEulerAngles(const cv::Mat& R);
    cv::Vec4d rotationMatrixToQuaternion(const cv::Mat& R);
}

PnPSolver::PnPSolver() {}

PnPSolver::PnPSolver(const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs)
    : camera_matrix_(camera_matrix), dist_coeffs_(dist_coeffs) {}

PnPSolver::~PnPSolver() {}

void PnPSolver::setCameraParams(const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs) {
    camera_matrix_ = camera_matrix.clone();
    dist_coeffs_ = dist_coeffs.clone();
}

bool PnPSolver::solve(const std::vector<cv::Point3f>& object_points,
                      const std::vector<cv::Point2f>& image_points,
                      cv::Mat& rotation_vector,
                      cv::Mat& translation_vector,
                      bool use_extrinsic_guess,
                      int flags) {
    if (object_points.size() != image_points.size() || object_points.size() < 4) {
        std::cerr << "PnP解算错误：需要至少4个点对" << std::endl;
        return false;
    }
    
    if (camera_matrix_.empty() || dist_coeffs_.empty()) {
        std::cerr << "PnP解算错误：相机参数未设置" << std::endl;
        return false;
    }
    
    try {
        return cv::solvePnP(object_points, image_points, camera_matrix_, dist_coeffs_,
                           rotation_vector, translation_vector, use_extrinsic_guess, flags);
    } catch (const cv::Exception& e) {
        std::cerr << "PnP解算异常: " << e.what() << std::endl;
        return false;
    }
}

cv::Mat PnPSolver::getRotationMatrix(const cv::Mat& rotation_vector) {
    cv::Mat rotation_matrix;
    cv::Rodrigues(rotation_vector, rotation_matrix);
    return rotation_matrix;
}

cv::Vec3d PnPSolver::getEulerAngles(const cv::Mat& rotation_vector) {
    cv::Mat R = getRotationMatrix(rotation_vector);
    return rotationMatrixToEulerAngles(R);
}

cv::Vec4d PnPSolver::getQuaternion(const cv::Mat& rotation_vector) {
    cv::Mat R = getRotationMatrix(rotation_vector);
    return rotationMatrixToQuaternion(R);
}

bool PnPSolver::verifyResult(const std::vector<cv::Point3f>& object_points,
                            const std::vector<cv::Point2f>& image_points,
                            const cv::Mat& rotation_vector,
                            const cv::Mat& translation_vector,
                            double max_reprojection_error) {
    std::vector<cv::Point2f> projected_points;
    cv::projectPoints(object_points, rotation_vector, translation_vector,
                     camera_matrix_, dist_coeffs_, projected_points);
    
    double total_error = 0.0;
    for (size_t i = 0; i < image_points.size(); i++) {
        double error = cv::norm(image_points[i] - projected_points[i]);
        total_error += error;
        if (error > max_reprojection_error) {
            return false;
        }
    }
    
    double avg_error = total_error / image_points.size();
    std::cout << "平均重投影误差: " << avg_error << " 像素" << std::endl;
    
    return avg_error <= max_reprojection_error;
}

// 内部辅助函数实现
namespace {
    cv::Vec3d rotationMatrixToEulerAngles(const cv::Mat& R) {
        cv::Mat R_double;
        if (R.type() != CV_64F) {
            R.convertTo(R_double, CV_64F);
        } else {
            R_double = R;
        }
        
        double sy = sqrt(R_double.at<double>(0,0) * R_double.at<double>(0,0) + 
                         R_double.at<double>(1,0) * R_double.at<double>(1,0));
        
        bool singular = sy < 1e-6;
        
        double x, y, z;
        if (!singular) {
            x = atan2(R_double.at<double>(2,1), R_double.at<double>(2,2));
            y = atan2(-R_double.at<double>(2,0), sy);
            z = atan2(R_double.at<double>(1,0), R_double.at<double>(0,0));
        } else {
            x = atan2(-R_double.at<double>(1,2), R_double.at<double>(1,1));
            y = atan2(-R_double.at<double>(2,0), sy);
            z = 0;
        }
        
        // 转换为角度
        const double PI = 3.14159265358979323846;
        x = x * 180.0 / PI;
        y = y * 180.0 / PI;
        z = z * 180.0 / PI;
        
        return cv::Vec3d(x, y, z);
    }
    
    cv::Vec4d rotationMatrixToQuaternion(const cv::Mat& R) {
        cv::Mat R_double;
        if (R.type() != CV_64F) {
            R.convertTo(R_double, CV_64F);
        } else {
            R_double = R;
        }
        
        double trace = R_double.at<double>(0,0) + R_double.at<double>(1,1) + R_double.at<double>(2,2);
        double qw, qx, qy, qz;
        
        if (trace > 0) {
            double S = sqrt(trace + 1.0) * 2.0;
            qw = 0.25 * S;
            qx = (R_double.at<double>(2,1) - R_double.at<double>(1,2)) / S;
            qy = (R_double.at<double>(0,2) - R_double.at<double>(2,0)) / S;
            qz = (R_double.at<double>(1,0) - R_double.at<double>(0,1)) / S;
        } else if (R_double.at<double>(0,0) > R_double.at<double>(1,1) && 
                   R_double.at<double>(0,0) > R_double.at<double>(2,2)) {
            double S = sqrt(1.0 + R_double.at<double>(0,0) - R_double.at<double>(1,1) - R_double.at<double>(2,2)) * 2.0;
            qw = (R_double.at<double>(2,1) - R_double.at<double>(1,2)) / S;
            qx = 0.25 * S;
            qy = (R_double.at<double>(0,1) + R_double.at<double>(1,0)) / S;
            qz = (R_double.at<double>(0,2) + R_double.at<double>(2,0)) / S;
        } else if (R_double.at<double>(1,1) > R_double.at<double>(2,2)) {
            double S = sqrt(1.0 + R_double.at<double>(1,1) - R_double.at<double>(0,0) - R_double.at<double>(2,2)) * 2.0;
            qw = (R_double.at<double>(0,2) - R_double.at<double>(2,0)) / S;
            qx = (R_double.at<double>(0,1) + R_double.at<double>(1,0)) / S;
            qy = 0.25 * S;
            qz = (R_double.at<double>(1,2) + R_double.at<double>(2,1)) / S;
        } else {
            double S = sqrt(1.0 + R_double.at<double>(2,2) - R_double.at<double>(0,0) - R_double.at<double>(1,1)) * 2.0;
            qw = (R_double.at<double>(1,0) - R_double.at<double>(0,1)) / S;
            qx = (R_double.at<double>(0,2) + R_double.at<double>(2,0)) / S;
            qy = (R_double.at<double>(1,2) + R_double.at<double>(2,1)) / S;
            qz = 0.25 * S;
        }
        
        return cv::Vec4d(qw, qx, qy, qz);
    }
}