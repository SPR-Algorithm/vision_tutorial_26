#include "camera.hpp"

#include <stdexcept>

#include "hikrobot/hikrobot.hpp"

namespace io
{
Camera::Camera(double exposure_ms, double gain, const std::string & vid_pid)
{
  camera_ = std::make_unique<HikRobot>(exposure_ms, gain, vid_pid);
}

void Camera::read(cv::Mat & img, std::chrono::steady_clock::time_point & timestamp)
{
  camera_->read(img, timestamp);
}

}  // namespace io