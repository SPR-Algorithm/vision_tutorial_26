#include <opencv2/opencv.hpp>
#include "ArmorDetector.h"

using namespace cv;
using namespace std;

//  完全复刻原主函数
int main() {
    VideoCapture cap("video.avi");
    if (!cap.isOpened()) {
        printf("无法打开video.avi！\n检查项：\n1. 视频与exe同目录\n2. 格式为AVI/MP4\n3. 文件未损坏\n");
        system("pause");
        return -1;
    }

    //创建装甲板检测器实例
    ArmorDetector detector;
    
    Mat frame, result_frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cout << "视频处理完毕！" << endl;
            break;
        }

        // 调用检测器的帧处理函数
        detector.processFrame(frame, result_frame);

        imshow("1.原图", frame);
        imshow("4.装甲板识别结果", result_frame);

        int key = waitKey(30);
        if (key == 27) {
            cout << "用户手动退出。" << endl;
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