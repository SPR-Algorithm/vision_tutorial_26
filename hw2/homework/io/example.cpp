#include "hikrobot/include/MvCameraControl.h"
#include <opencv2/opencv.hpp>

cv::Mat transfer(MV_FRAME_OUT& raw)
{
    MV_CC_PIXEL_CONVERT_PARAM cvt_param;
    cv::Mat img(cv::Size(raw.stFrameInfo.nWidth, raw.stFrameInfo.nHeight), CV_8U, raw.pBufAddr);

    cvt_param.nWidth = raw.stFrameInfo.nWidth;
    cvt_param.nHeight = raw.stFrameInfo.nHeight;

    cvt_param.pSrcData = raw.pBufAddr;
    cvt_param.nSrcDataLen = raw.stFrameInfo.nFrameLen;
    cvt_param.enSrcPixelType = raw.stFrameInfo.enPixelType;

    cvt_param.pDstBuffer = img.data;
    cvt_param.nDstBufferSize = img.total() * img.elemSize();
    cvt_param.enDstPixelType = PixelType_Gvsp_BGR8_Packed;

    auto pixel_type = raw.stFrameInfo.enPixelType;
    const static std::unordered_map<MvGvspPixelType, cv::ColorConversionCodes> type_map = {
      {PixelType_Gvsp_BayerGR8, cv::COLOR_BayerGR2RGB},
      {PixelType_Gvsp_BayerRG8, cv::COLOR_BayerRG2RGB},
      {PixelType_Gvsp_BayerGB8, cv::COLOR_BayerGB2RGB},
      {PixelType_Gvsp_BayerBG8, cv::COLOR_BayerBG2RGB}};
    cv::cvtColor(img, img, type_map.at(pixel_type));
    
    return img;
}
int main()
{ 
    // 打开相机
    void * handle;
    int ret;
    MV_CC_DEVICE_INFO_LIST device_list;
    ret = MV_CC_EnumDevices(MV_USB_DEVICE, &device_list);
    if (ret != MV_OK) {
      return -1;
    }
  
    if (device_list.nDeviceNum == 0) {
      return -1;
    }
  
    ret = MV_CC_CreateHandle(&handle, device_list.pDeviceInfo[0]);
    if (ret != MV_OK) {
      return -1;
    }
  
    ret = MV_CC_OpenDevice(handle);
    if (ret != MV_OK) {
      return -1;
    }
  
    MV_CC_SetEnumValue(handle, "BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_CONTINUOUS);
    MV_CC_SetEnumValue(handle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    MV_CC_SetEnumValue(handle, "GainAuto", MV_GAIN_MODE_OFF);
    MV_CC_SetFloatValue(handle, "ExposureTime", 10000);
    MV_CC_SetFloatValue(handle, "Gain", 20);
    MV_CC_SetFrameRate(handle, 60);
   
    // 读取一帧图像
    ret = MV_CC_StartGrabbing(handle);
    if (ret != MV_OK) {
      return -1;
    }
  
    MV_FRAME_OUT raw;
    unsigned int nMsec = 100;

    ret = MV_CC_GetImageBuffer(handle, &raw, nMsec);
    if (ret != MV_OK) {
      return -1;
    }

    cv::Mat img = transfer(raw);
    cv::imshow("img", img);
    cv::waitKey(0);

    ret = MV_CC_FreeImageBuffer(handle, &raw);
    if (ret != MV_OK) {
      return -1;
    }
    
    // 关闭相机
    ret = MV_CC_StopGrabbing(handle);
    if (ret != MV_OK) {
        return -1;
    }

    ret = MV_CC_CloseDevice(handle);
    if (ret != MV_OK) {
        return -1;
    }

    ret = MV_CC_DestroyHandle(handle);
    if (ret != MV_OK) {
        return -1;
    }

}
