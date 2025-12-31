#pragma once
#include <opencv2/opencv.hpp>

class PnPSolver {
public:
    PnPSolver();
    explicit PnPSolver(const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs);
    ~PnPSolver();
    
    void setCameraParams(const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs);
    
    bool solve(const std::vector<cv::Point3f>& object_points,
               const std::vector<cv::Point2f>& image_points,
               cv::Mat& rotation_vector,
               cv::Mat& translation_vector,
               bool use_extrinsic_guess = false,
               int flags = cv::SOLVEPNP_ITERATIVE);
    
    cv::Mat getRotationMatrix(const cv::Mat& rotation_vector);
    cv::Vec3d getEulerAngles(const cv::Mat& rotation_vector);
    cv::Vec4d getQuaternion(const cv::Mat& rotation_vector);
    
    bool verifyResult(const std::vector<cv::Point3f>& object_points,
                      const std::vector<cv::Point2f>& image_points,
                      const cv::Mat& rotation_vector,
                      const cv::Mat& translation_vector,
                      double max_reprojection_error = 5.0);
    
private:
    cv::Mat camera_matrix_;
    cv::Mat dist_coeffs_;
};