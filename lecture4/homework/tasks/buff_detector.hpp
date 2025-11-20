#ifndef AUTO_BUFF__TRACK_HPP
#define AUTO_BUFF__TRACK_HPP

#include "buff_type.hpp"
#include "tools/img_tools.hpp"
#include "yolo11_buff.hpp"
namespace auto_buff
{
class Buff_Detector
{
public:
  Buff_Detector();
  std::vector<FanBlade> detect(cv::Mat & bgr_img);
private:
  cv::Point2f get_r_center(std::vector<FanBlade> & fanblades, cv::Mat & bgr_img);
  YOLO11_BUFF MODE_;
};
}  // namespace auto_buff
#endif  // DETECTOR_HPP