#include "buff_detector.hpp"


namespace auto_buff
{
Buff_Detector::Buff_Detector() : MODE_() {}


std::vector<FanBlade> Buff_Detector::detect(cv::Mat & bgr_img)
{

  std::vector<YOLO11_BUFF::Object> results = MODE_.get_onecandidatebox(bgr_img);
  if (results.empty()) {
    return std::vector<FanBlade>();
  }

  std::vector<FanBlade> fanblades;
  auto result = results[0];
  fanblades.emplace_back(FanBlade(result.kpt, result.kpt[4], _light));

  return fanblades;
}
}  // namespace auto_buff