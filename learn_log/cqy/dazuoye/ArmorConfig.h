ifndef ARMOR_CONFIG_H
#define ARMOR_CONFIG_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class ArmorConfig
{
public:
    
    inline static const float ANGLE_THRESHOLD = 45.0f;      // 角度差阈值
    inline static const float AREA_THRESHOLD = 150.0f;       // 面积阈值
    inline static const float RATIO_THRESHOLD = 2.0f;        // 长宽比阈值
    inline static const int BINARY_THRESH = 230;            // 二值化阈值
    inline static const float EPS = 1e-6f;                  // 防除0极小值

    
    inline static const cv::Mat camera_matrix =              // 相机内参矩阵
        (cv::Mat_<double>(3, 3) <<  1286.307063384126 , 0                  , 645.34450819155256, 
                                    0                 , 1288.1400736562441 , 483.6163720308021 , 
                                    0                 , 0                  , 1                   );

    inline static const cv::Mat distort_coeffs =             // 畸变系数
        (cv::Mat_<double>(1, 5) << -0.47562935060124745, 0.21831745829617311, 0.0004957613589406044, -0.00034617769548693592, 0);

    inline static const double LIGHTBAR_LENGTH = 0.056;     // 灯条长度 单位：米，一字未改
    inline static const double ARMOR_WIDTH = 0.135;         // 装甲板宽度

    // 装甲板3D模型点：完全保留原始坐标
    inline static const vector<Point3f> armor_3d_points = {
        Point3f(-ARMOR_WIDTH/2,  LIGHTBAR_LENGTH/2, 0.0),  // 左上
        Point3f(ARMOR_WIDTH/2,   LIGHTBAR_LENGTH/2, 0.0),  // 右上
        Point3f(ARMOR_WIDTH/2,  -LIGHTBAR_LENGTH/2, 0.0),  // 右下
        Point3f(-ARMOR_WIDTH/2, -LIGHTBAR_LENGTH/2, 0.0)   // 左下
    };
};

#endif // ARMOR_CONFIG_H