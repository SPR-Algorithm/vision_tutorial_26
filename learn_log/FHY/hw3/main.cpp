#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

void processFrame(const Mat& frame, Mat& result) {
    //灰度 → 二值化→ 找轮廓→ 筛选灯条→ 绘制
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
    imshow("2.灰度图", gray);
    imshow("3.二值图", binary);
}

int main() {
    VideoCapture cap("demo.avi");
    // 检查视频是否打开成功
    if (!cap.isOpened()) {
        printf("无法打开视频！请检查路径：\n");
        return -1;
    }

    Mat frame, result_frame;
    while (true) {
        // 逐帧读取视频（扣帧）
        cap >> frame;
        if (frame.empty()) { // 视频结束
            cout << "视频处理完毕" << endl;
            break;
        }

        // 处理当前帧
        processFrame(frame, result_frame);
        imshow("1.原图", frame);
        imshow("4.灯条识别结果", result_frame);

        // 按 ESC 键退出
        if (waitKey(30) == 27) {
            cout << "用户手动退出。" << endl;
            break;
        }
    }

    // 释放资源
    cap.release();
    destroyAllWindows();
    return 0;
}
