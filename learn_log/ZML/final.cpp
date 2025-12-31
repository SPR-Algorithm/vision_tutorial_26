
#include <stdio.h>
#include <stdlib.h>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
    VideoCapture cap("/home/a/final/video.avi");
    
    Mat frame, gray, binary;

    
    const double MIN_ASPECT_RATIO = 6;  
    const double MAX_ASPECT_RATIO = 6.5;  
    const double MIN_AREA = 100.0;        
    
    while (cap.isOpened()) {
        cap >> frame;
        
        if (frame.empty() || waitKey(1) == 27) {
            break;
        }
        
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 127, 255, THRESH_BINARY);
        
        // 查找轮廓
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        // 为每个轮廓拟合椭圆并进行筛选
        for (size_t i = 0; i < contours.size(); i++) {
            // 1. 检查轮廓是否足够用于椭圆拟合（至少需要5个点）
            if (contours[i].size() < 5) {
                continue;
            }
            
            // 2. 检查轮廓面积是否足够大
            double area = contourArea(contours[i]);
            if (area < MIN_AREA) {
                continue;
            }
            
            try {
                
                RotatedRect ellipse = fitEllipse(contours[i]);
                
                
                double majorAxis = max(ellipse.size.width, ellipse.size.height);  
                double minorAxis = min(ellipse.size.width, ellipse.size.height); 
                
               
                if (minorAxis == 0) {
                    continue;
                }
                
                double aspectRatio = majorAxis / minorAxis;
                
                
                if (aspectRatio < MIN_ASPECT_RATIO || aspectRatio > MAX_ASPECT_RATIO) {
                    continue;
                }
                
                
                cv::ellipse(frame, ellipse, Scalar(0, 255, 0), 2);
                
                
            
                        
            } catch (const Exception& e) {
                
            }
        }
        
        
        namedWindow("Final", WINDOW_AUTOSIZE);
        imshow("Final", frame);
    }

    cap.release();
    destroyAllWindows();
    
    
}