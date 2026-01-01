#include "ArmorDetector.h"

// 灯条配对有效性校验
bool ArmorDetector::isPairValid(const LightBar& bar1, const LightBar& bar2, float& total_score) const {
    RotatedRect r1 = bar1.getRect(); 
    RotatedRect r2 = bar2.getRect();
    
    float angle1 = ToolUtils::correctAngle(r1);
    float angle2 = ToolUtils::correctAngle(r2);
    float angle_diff = abs(angle1 - angle2);
    float center_dist = ToolUtils::calcDistance(r1.center, r2.center);
    
    float height1 = bar1.getHeight();
    float height2 = bar2.getHeight();
    float avg_height = (height1 + height2) * 0.5f;
    
    if (max(height1, height2) / min(height1, height2) > 2.0f) return false;
    if (abs(r1.center.x - r2.center.x) < abs(r1.center.y - r2.center.y)) return false;
    if (center_dist < avg_height * 0.5f || center_dist > avg_height * 5.0f) return false;
    if (angle_diff > ArmorConfig::ANGLE_THRESHOLD) return false;
    return (true);
}


int ArmorDetector::selectBestPair(const vector<pair<LightBar, LightBar>>& all_pairs) const {
    int best_idx = 0;
    float max_score = 0.0f;
    for (size_t k = 0; k < all_pairs.size(); k++) {
        const LightBar& bar1 = all_pairs[k].first;
        const LightBar& bar2 = all_pairs[k].second;
        
        float height1 = bar1.getHeight();
        float height2 = bar2.getHeight();
        float height_sim = 1.0f - abs(height1 - height2) / (max(height1, height2) + ArmorConfig::EPS);
        
        float center_dist = ToolUtils::calcDistance(bar1.getCenter(), bar2.getCenter());
        float avg_height = (height1 + height2) * 0.5f;
        float dist_score = 1.0f - abs(center_dist - avg_height * 2.0f) / (avg_height * 2.0f + ArmorConfig::EPS);
        
        float angle1 = ToolUtils::correctAngle(bar1.getRect());
        float angle2 = ToolUtils::correctAngle(bar2.getRect());
        float angle_diff = abs(angle1 - angle2);
        float angle_score = 1.0f - angle_diff / ArmorConfig::ANGLE_THRESHOLD;
        
        float total_score = height_sim * 0.4f + dist_score * 0.3f + angle_score * 0.3f;
        if (total_score > max_score) {
            max_score = total_score;
            best_idx = k;
        }
    }
    return best_idx;
}

void ArmorDetector::processFrame(const Mat& frame, Mat& result) {
    result = frame.clone();
    Mat binary;
    
    //预处理
    Mat bgr[3];
    split(frame, bgr);
    Mat r_channel = bgr[2];
    GaussianBlur(r_channel, r_channel, Size(5, 5), 0);
    threshold(r_channel, binary, ArmorConfig::BINARY_THRESH, 255, THRESH_BINARY);
    
    //查找轮廓
    vector<vector<Point>> contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    //筛选灯条,用LightBar类
    vector<LightBar> light_bars;
    for (size_t i = 0; i < contours.size(); i++) {
        LightBar bar(contours[i]);
        if (bar.isValid()) {
            light_bars.push_back(bar);
        }
    }
    
    //匹配灯条对
    vector<pair<LightBar, LightBar>> all_pairs;
    for (size_t i = 0; i < light_bars.size(); i++) {
        for (size_t j = i + 1; j < light_bars.size(); j++) {
            float total_score = 0.0f;
            if (isPairValid(light_bars[i], light_bars[j], total_score)) {
                all_pairs.push_back({light_bars[i], light_bars[j]});
            }
        }
    }
    
    //绘制装甲板 保留绿框+PnP结果
    if (!all_pairs.empty()) {
        int best_idx = selectBestPair(all_pairs);
        auto& pair = all_pairs[best_idx];
        LightBar left_bar = pair.first;
        LightBar right_bar = pair.second;
        
        RotatedRect left = left_bar.getRect();
        RotatedRect right = right_bar.getRect();
        if (left.center.x >right.center.x) {
            swap(left, right);
            swap(left_bar,right_bar);
        }
       

        //绘制绿框
        Point2f left_pts[4], right_pts[4];
        left.points(left_pts);
        right.points(right_pts);
        float current_angle=right.angle;
        Point2f left_top, left_bottom, right_top, right_bottom;
        ToolUtils::sortVerticesByY(left_pts, left_top, left_bottom);
        ToolUtils::sortVerticesByY(right_pts, right_top, right_bottom);
        
        float avg_height = (ToolUtils::calcDistance(left_top, left_bottom) + 
                          ToolUtils::calcDistance(right_top, right_bottom)) * 0.5f;
        float horizontal_extend = avg_height * 0.1f;
        float vertical_extend = avg_height * 0.1f;
        
        vector<Point2f> armor_pts(4);
        armor_pts[0] = Point2f(left_top.x - horizontal_extend, left_top.y - vertical_extend);
        armor_pts[1] = Point2f(right_top.x + horizontal_extend, right_top.y - vertical_extend);
        armor_pts[2] = Point2f(right_bottom.x + horizontal_extend, right_bottom.y + vertical_extend);
        armor_pts[3] = Point2f(left_bottom.x - horizontal_extend, left_bottom.y + vertical_extend);
        
        for (int i = 0; i < 4; i++) {
            int next = (i + 1) % 4;
            line(result, armor_pts[i], armor_pts[next], Scalar(0, 255, 0), 2);
        }
    
        // PnP解算
        Mat rvec, tvec;
        solvePnP(ArmorConfig::armor_3d_points, armor_pts, ArmorConfig::camera_matrix, 
                 ArmorConfig::distort_coeffs, rvec, tvec);
         float distance = static_cast<float>(norm(tvec));

         // 计算装甲板绿框水平中点
        float centerX = (armor_pts[0].x + armor_pts[1].x) / 2;
        float centerY = (armor_pts[0].y + armor_pts[1].y) / 2;
        // 绘制armoredplate文字
        putText(result, "Dog", Point(centerX-60,centerY-10), 
        FONT_HERSHEY_SIMPLEX, (1-distance), Scalar(0,0,255), 2);
        
        // 左上角显示PnP结果 品红
        string info_rvec = "Rvec: " + to_string(rvec.at<double>(0)) + ", " + to_string(rvec.at<double>(1)) + ", " + to_string(rvec.at<double>(2));
        string info_tvec = "Tvec(m): " + to_string(tvec.at<double>(0)) + ", " + to_string(tvec.at<double>(1)) + ", " + to_string(tvec.at<double>(2));
        string info_dist = "Distance: " + to_string(distance) + " m";
        
        putText(result, info_rvec, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 255), 2);
        putText(result, info_tvec, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 255), 2);
        putText(result, info_dist, Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 255), 2);
    }
}