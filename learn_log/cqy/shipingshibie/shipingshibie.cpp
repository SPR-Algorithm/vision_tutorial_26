#include "shipingshibie.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
void processFrame(const Mat& frame, Mat& result) {
    result = frame.clone();
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    Mat binary;
    threshold(gray, binary, 127, 255, THRESH_BINARY);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<RotatedRect> rotated_rects;
    for (auto& cnt : contours) {
        RotatedRect r_rect = minAreaRect(cnt);
        rotated_rects.push_back(r_rect);
    }
    for (auto& r_rect : rotated_rects) {
        float width = min(r_rect.size.width, r_rect.size.height);
        float height = max(r_rect.size.width, r_rect.size.height);
        float ratio = height / width;
        if (r_rect.size.area() > 50 && ratio > 2) {
            Point2f points[4];
            r_rect.points(points);
            for (int i = 0; i < 4; i++) {
                line(result, points[i], points[(i + 1) % 4], Scalar(0, 255, 0), 2);
            }
        }
    }
    imshow("2.»Ò¶ÈÍ¼", gray);
    imshow("3.¶þÖµÍ¼", binary);
}