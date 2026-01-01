#include <opencv2/opencv.hpp>
#include "ArmorDetector.h"

using namespace cv;
using namespace std;
int main() {
    VideoCapture cap("video.avi");
    if (!cap.isOpened()) {
        printf("无法打开video.avi");
        system("pause");
        return -1;
    }

    // 创建装甲板检测器实例
    ArmorDetector detector;
    
    Mat frame, result_frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cout << "视频处理完毕" << endl;
            break;
        }

        // 调用检测器的帧处理函数
        detector.processFrame(frame, result_frame);
        imshow("装甲板识别结果", result_frame);

        int key = waitKey(30);
        if (key == 27) {
            cout << "手动退出。" << endl;
            break;
        } else if (key == 32) {
            waitKey(0);
        }
    }

    cap.release();
    destroyAllWindows();
    system("pause");
    return 0;
}