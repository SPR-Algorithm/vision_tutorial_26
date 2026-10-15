#ifndef IO__MY_CAMERA_HPP
#define IO__MY_CAMERA_HPP

#include <opencv2/opencv.hpp>

#include <string>

// 前向声明海康 SDK 的帧结构体，避免把 SDK 头文件暴露给使用者的编译单元
struct _MV_FRAME_OUT_;
typedef struct _MV_FRAME_OUT_ MV_FRAME_OUT;

namespace io
{

// 相机参数，默认值取自 io/example.cpp
struct CameraConfig
{
  bool balance_white_auto = true;   // 自动白平衡
  bool exposure_auto = false;       // 自动曝光（要手动写 ExposureTime 必须关闭）
  bool gain_auto = false;           // 自动增益（要手动写 Gain 必须关闭）
  double exposure_time = 10000.0;   // 曝光时间，单位 us
  double gain = 20.0;               // 增益，单位 dB
  double frame_rate = 60.0;         // 采集帧率，单位 fps
  int flip_code = 2;                // 图像翻转，取值同 cv::flip；99 表示不翻转
  int device_index = 0;             // 使用第几个枚举到的设备
  unsigned int timeout_ms = 1000;   // 取流超时，单位 ms
};

// 相机类：封装 hikrobot(MVS) SDK 的调用流程
// 使用流程：open() -> read() ... read() -> close()
// 析构时自动 close()，保证 StopGrabbing / CloseDevice / DestroyHandle 一定被调用
class MyCamera
{
public:
  MyCamera() = default;
  // 从 yaml 读取相机参数；文件不存在或字段缺失时回退到默认值
  explicit MyCamera(const std::string & config_path);
  ~MyCamera();

  // 句柄不可拷贝
  MyCamera(const MyCamera &) = delete;
  MyCamera & operator=(const MyCamera &) = delete;

  bool open();                // 枚举设备 -> 创建句柄 -> 打开设备 -> 设置参数 -> 开始取流
  void close();               // 停止取流 -> 关闭设备 -> 销毁句柄
  bool isOpened() const;

  bool read(cv::Mat & img);   // 取一帧并转换为 OpenCV 的 BGR 图像
  MyCamera & operator>>(cv::Mat & img);

  int width() const;
  int height() const;
  double frameRate() const;

private:
  CameraConfig config_;
  void * handle_ = nullptr;
  bool opened_ = false;
  int width_ = 0;
  int height_ = 0;

  bool check(int ret, const std::string & action);   // 统一错误检查与输出
  bool setParams();                                   // 曝光 / 增益 / 白平衡 / 帧率
  bool transfer(MV_FRAME_OUT & raw, cv::Mat & img);   // 原始帧 -> cv::Mat
};

}  // namespace io

#endif  // IO__MY_CAMERA_HPP
