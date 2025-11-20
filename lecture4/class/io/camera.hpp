#ifndef IO_CAMERA__HPP
#define IO_CAMERA__HPP

#include <CameraApi.h>

#include <opencv2/opencv.hpp>

namespace io
{
    class Camera
    {
    public:
        Camera(int exposure);
        ~Camera();
        void read(cv::Mat &img);

    private:
        int exposure_ms_;
        int iCameraCounts = 1;
        int iStatus = -1;
        tSdkCameraDevInfo tCameraEnumList;
        int hCamera;
        tSdkCameraCapbility tCapability; // 设备描述信息
        tSdkFrameHead sFrameInfo;
        BYTE *pbyBuffer;
        int iDisplayFrames = 10000;
        int channel = 3;
        unsigned char *g_pRgbBuffer;
    };
} // namespace io

#endif