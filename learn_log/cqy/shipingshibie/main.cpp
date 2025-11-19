#include <iostream>
#include <opencv2/opencv.hpp>
#include "shipingshibie.h"
using namespace cv;
using namespace std;

int main() { 
    VideoCapture cap("demo.avi");
    if (!cap.isOpened()) {
        printf(" 无法打开视频！请检查：\n");
        printf("1. 路径是否正确：%s\n", "/home/rsev/Desktop/LightBarDetection/demo.avi");
        printf("2. 视频文件是否存在（文件名是否为 demo.avi，注意大小写）\n");
        printf("3. 程序是否有读取该路径的权限（可右键文件夹→属性→权限检查）\n");
        return -1;
    }

    Mat frame, result_frame;
    while (true) {
        // 逐帧读取视频（扣帧）
        cap >> frame;
        if (frame.empty()) { // 视频结束
            cout << "视频处理完毕！" << endl;
            break;
        }

        // 处理当前帧（保留所有原功能）
        processFrame(frame, result_frame);

        // 显示窗口（自动创建，无需提前初始化）
        imshow("1.原图", frame);
        imshow("4.灯条识别结果", result_frame);

        // 按 ESC 键退出
        if (waitKey(30) == 27) {
            cout << " 用户手动退出。" << endl;
            break;
        }
    }

    // 释放资源
    cap.release();
    destroyAllWindows();
    return 0;
}