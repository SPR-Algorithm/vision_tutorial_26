#include "my_camera.hpp"

#include <yaml-cpp/yaml.h>

#include <cstring>
#include <iostream>
#include <stdexcept>

#include "hikrobot/include/MvCameraControl.h"

namespace io
{

MyCamera::MyCamera(const std::string & config_path)
{
  try {
    auto yaml = YAML::LoadFile(config_path);

    if (yaml["device_index"]) config_.device_index = yaml["device_index"].as<int>();
    if (yaml["flip_code"]) config_.flip_code = yaml["flip_code"].as<int>();
    if (yaml["frame_rate"]) config_.frame_rate = yaml["frame_rate"].as<double>();
    if (yaml["gain"]) config_.gain = yaml["gain"].as<double>();
    if (yaml["exposure_auto"]) config_.exposure_auto = yaml["exposure_auto"].as<bool>();
    if (yaml["gain_auto"]) config_.gain_auto = yaml["gain_auto"].as<bool>();
    if (yaml["balance_white_auto"]) config_.balance_white_auto = yaml["balance_white_auto"].as<bool>();
    if (yaml["timeout_ms"]) config_.timeout_ms = yaml["timeout_ms"].as<unsigned int>();

    // 曝光优先用 us，其次兼容 ms
    if (yaml["exposure_time"]) {
      config_.exposure_time = yaml["exposure_time"].as<double>();
    } else if (yaml["exposure_ms"]) {
      config_.exposure_time = yaml["exposure_ms"].as<double>() * 1000.0;
    }
  } catch (const std::exception & e) {
    std::cerr << "[MyCamera] Load config '" << config_path << "' failed: " << e.what()
              << ", use default parameters." << std::endl;
  }
}

MyCamera::~MyCamera()
{
  close();
}

bool MyCamera::open()
{
  if (opened_) return true;

  // 1. 枚举 GigE / USB 设备
  MV_CC_DEVICE_INFO_LIST device_list;
  std::memset(&device_list, 0, sizeof(device_list));
  if (!check(MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &device_list), "EnumDevices")) {
    return false;
  }
  if (device_list.nDeviceNum == 0) {
    std::cerr << "[MyCamera] No camera found!" << std::endl;
    return false;
  }
  if (config_.device_index < 0 || config_.device_index >= static_cast<int>(device_list.nDeviceNum)) {
    std::cerr << "[MyCamera] device_index " << config_.device_index << " out of range, only "
              << device_list.nDeviceNum << " device(s) found!" << std::endl;
    return false;
  }

  // 2. 创建句柄
  if (!check(
        MV_CC_CreateHandle(&handle_, device_list.pDeviceInfo[config_.device_index]),
        "CreateHandle")) {
    handle_ = nullptr;
    return false;
  }

  // 3. 打开设备
  if (!check(MV_CC_OpenDevice(handle_), "OpenDevice")) {
    close();
    return false;
  }

  // 4. 设置相机参数
  if (!setParams()) {
    close();
    return false;
  }

  // 5. 开始取流
  if (!check(MV_CC_StartGrabbing(handle_), "StartGrabbing")) {
    close();
    return false;
  }

  opened_ = true;
  return true;
}

void MyCamera::close()
{
  if (!handle_) return;

  if (opened_) {
    check(MV_CC_StopGrabbing(handle_), "StopGrabbing");
    opened_ = false;
  }

  check(MV_CC_CloseDevice(handle_), "CloseDevice");
  check(MV_CC_DestroyHandle(handle_), "DestroyHandle");

  handle_ = nullptr;
  width_ = height_ = 0;
}

bool MyCamera::isOpened() const
{
  return opened_;
}

bool MyCamera::read(cv::Mat & img)
{
  if (!opened_) {
    std::cerr << "[MyCamera] Camera not opened, call open() first!" << std::endl;
    return false;
  }

  MV_FRAME_OUT raw;
  std::memset(&raw, 0, sizeof(raw));

  // 超时属正常现象，不打印日志以免刷屏
  if (MV_CC_GetImageBuffer(handle_, &raw, config_.timeout_ms) != MV_OK) {
    return false;
  }

  bool ok = transfer(raw, img);

  // GetImageBuffer 与 FreeImageBuffer 必须成对调用
  if (!check(MV_CC_FreeImageBuffer(handle_, &raw), "FreeImageBuffer")) {
    return false;
  }

  return ok;
}

MyCamera & MyCamera::operator>>(cv::Mat & img)
{
  read(img);
  return *this;
}

int MyCamera::width() const
{
  return width_;
}

int MyCamera::height() const
{
  return height_;
}

double MyCamera::frameRate() const
{
  return config_.frame_rate;
}

// ---------------- private ----------------

bool MyCamera::check(int ret, const std::string & action)
{
  if (ret != MV_OK) {
    std::cerr << "[MyCamera] " << action << " failed! error code: 0x" << std::hex << ret << std::dec
              << std::endl;
    return false;
  }
  return true;
}

bool MyCamera::setParams()
{
  bool ok = true;

  // 连续采集：关闭触发模式
  ok &= check(MV_CC_SetEnumValue(handle_, "TriggerMode", MV_TRIGGER_MODE_OFF), "Set TriggerMode");

  // 白平衡
  ok &= check(
    MV_CC_SetEnumValue(
      handle_, "BalanceWhiteAuto",
      config_.balance_white_auto ? MV_BALANCEWHITE_AUTO_CONTINUOUS : MV_BALANCEWHITE_AUTO_OFF),
    "Set BalanceWhiteAuto");

  // 自动曝光必须先关掉，ExposureTime 才可写
  ok &= check(
    MV_CC_SetEnumValue(
      handle_, "ExposureAuto",
      config_.exposure_auto ? MV_EXPOSURE_AUTO_MODE_CONTINUOUS : MV_EXPOSURE_AUTO_MODE_OFF),
    "Set ExposureAuto");

  // 自动增益必须先关掉，Gain 才可写
  ok &= check(
    MV_CC_SetEnumValue(
      handle_, "GainAuto", config_.gain_auto ? MV_GAIN_MODE_CONTINUOUS : MV_GAIN_MODE_OFF),
    "Set GainAuto");

  if (!config_.exposure_auto) {
    ok &= check(
      MV_CC_SetFloatValue(handle_, "ExposureTime", static_cast<float>(config_.exposure_time)),
      "Set ExposureTime");
  }

  if (!config_.gain_auto) {
    ok &= check(
      MV_CC_SetFloatValue(handle_, "Gain", static_cast<float>(config_.gain)), "Set Gain");
  }

  // 帧率：先使能再设置
  ok &= check(
    MV_CC_SetBoolValue(handle_, "AcquisitionFrameRateEnable", true), "Enable AcquisitionFrameRate");
  ok &= check(
    MV_CC_SetFloatValue(handle_, "AcquisitionFrameRate", static_cast<float>(config_.frame_rate)),
    "Set AcquisitionFrameRate");

  return ok;
}

bool MyCamera::transfer(MV_FRAME_OUT & raw, cv::Mat & img)
{
  auto width = static_cast<int>(raw.stFrameInfo.nWidth);
  auto height = static_cast<int>(raw.stFrameInfo.nHeight);
  if (width <= 0 || height <= 0) {
    std::cerr << "[MyCamera] Invalid frame size!" << std::endl;
    return false;
  }

  // 无论相机输出 Bayer / Mono / RGB，统一转成 BGR8
  img.create(cv::Size(width, height), CV_8UC3);

  MV_CC_PIXEL_CONVERT_PARAM_EX cvt_param;
  std::memset(&cvt_param, 0, sizeof(cvt_param));
  cvt_param.nWidth = raw.stFrameInfo.nWidth;
  cvt_param.nHeight = raw.stFrameInfo.nHeight;
  cvt_param.enSrcPixelType = raw.stFrameInfo.enPixelType;
  cvt_param.pSrcData = raw.pBufAddr;
  cvt_param.nSrcDataLen = raw.stFrameInfo.nFrameLen;
  cvt_param.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
  cvt_param.pDstBuffer = img.data;
  cvt_param.nDstBufferSize = static_cast<unsigned int>(img.total() * img.elemSize());

  if (!check(MV_CC_ConvertPixelTypeEx(handle_, &cvt_param), "ConvertPixelType")) {
    return false;
  }

  width_ = img.cols;
  height_ = img.rows;

  // 99 为自定义哨兵值，表示不做翻转
  if (config_.flip_code != 99) {
    cv::flip(img, img, config_.flip_code);
  }

  return true;
}

}  // namespace io
