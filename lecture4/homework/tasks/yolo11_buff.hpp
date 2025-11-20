#ifndef AUTO_BUFF__YOLO11_BUFF_HPP
#define AUTO_BUFF__YOLO11_BUFF_HPP

#include <filesystem>
#include <opencv2/opencv.hpp>
#include <openvino/openvino.hpp>


namespace auto_buff
{
const std::vector<std::string> class_names = {"buff", "r"};

class YOLO11_BUFF
{
public:
  struct Object
  {
    cv::Rect_<float> rect;
    int label;
    float prob;
    std::vector<cv::Point2f> kpt;
  };

  YOLO11_BUFF();

  std::vector<Object> get_multicandidateboxes(cv::Mat & image);

  std::vector<Object> get_onecandidatebox(cv::Mat & image);

private:
  ov::Core core;  
  std::shared_ptr<ov::Model> model;
  ov::CompiledModel compiled_model;
  ov::InferRequest infer_request;
  ov::Tensor input_tensor;
  const int NUM_POINTS = 6;

  void convert(
    const cv::Mat & input, cv::Mat & output, const bool normalize, const bool exchangeRB) const;

  float fill_tensor_data_image(ov::Tensor & input_tensor, const cv::Mat & input_image) const;

  void printInputAndOutputsInfo(const ov::Model & network);

  void save(const std::string & programName, const cv::Mat & image);
};
}  // namespace auto_buff
#endif