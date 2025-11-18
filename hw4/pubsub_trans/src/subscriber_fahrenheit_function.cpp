#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

class FahrenheitSubscriber : public rclcpp::Node
{
private:
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subscription_;
public:
    FahrenheitSubscriber()
    : Node("fahrenheit_subscriber")
    {
      //构造话题回调函数
      auto topic_callback = 
        [this](std_msgs::msg::Float32::UniquePtr msg) -> void {
            float fahrenheit = (msg->data * 9.0/5.0) + 32.0; //celsius convert to fahrenheit
            RCLCPP_INFO(this->get_logger(),"fahrenheit is: %.2fF", fahrenheit);
        };
      subscription_ =
        this->create_subscription<std_msgs::msg::Float32>("celsius",10,topic_callback);
    }
};
// auto topic_callback = 
//     [this](std_msgs::msg::Float32::UniquePtr msg) -> void {
//         float fahrenheit = (msg->data * 9.0/5.0) + 32.0;  // 摄氏转华氏
//         RCLCPP_INFO(this->get_logger(), "fahrenheit is: '%.2f'", fahrenheit);
//     };
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FahrenheitSubscriber>());
    rclcpp::shutdown();
    return 0;
}