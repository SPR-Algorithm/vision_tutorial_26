#include "tasks/buff_detector.hpp"
#include "io/camera.hpp"
#include <chrono>
#include <opencv2/opencv.hpp>

int main()
{
    io::Camera camera(2.5, 16.9, "2bdf:0001");
    std::chrono::steady_clock::time_point timestamp;  
    auto_buff::Buff_Detector detector;
    while(true){
        cv::Mat img;
        camera.read(img, timestamp);
        auto fanblades = detector.detect(img);
    }
    return 0;
}