#include "camera.hpp"

namespace io
{
    Camera::Camera(int exposure)
    {
        exposure_ms_ = exposure;
        CameraSdkInit(1);
        iStatus = CameraEnumerateDevice(&tCameraEnumList, &iCameraCounts);

        // 没有连接设备
        if (iCameraCounts == 0)
            throw std::runtime_error("Not found camera!");

        // 相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
        iStatus = CameraInit(&tCameraEnumList, -1, -1, &hCamera);
        CameraGetCapability(hCamera, &tCapability);
        g_pRgbBuffer = (unsigned char *)malloc(
            tCapability.sResolutionRange.iHeightMax * tCapability.sResolutionRange.iWidthMax * 3);
        CameraSetAeState(hCamera, FALSE); // 关闭自动曝光
        CameraSetExposureTime(hCamera, exposure_ms_ * 1e3);
        CameraSetIspOutFormat(hCamera, CAMERA_MEDIA_TYPE_BGR8);
        CameraPlay(hCamera);
    };

    void Camera::read(cv::Mat &img)
    {
        if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS)
        {
            CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer, &sFrameInfo);
            cv::Mat raw_img(cv::Size(sFrameInfo.iWidth, sFrameInfo.iHeight), CV_8UC3, g_pRgbBuffer);
            img = raw_img;
            CameraReleaseImageBuffer(hCamera, pbyBuffer);
        }
        else
            return;
    };

    Camera::~Camera()
    {
        CameraUnInit(hCamera);
        // 注意，现反初始化后再free
        free(g_pRgbBuffer);
    }

} // namespace io