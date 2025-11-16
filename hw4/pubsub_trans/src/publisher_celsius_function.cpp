#include <chrono>
#include <memory>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;
class CelsiusPublisher : public rclcpp::Node
{
public:
    CelsiusPublisher()
    : Node("celsius_publisher"),count_(0)
    {
        publisher_= this->create_publisher<std_msgs::msg::Float32>("/celsius",10);
        // 创建定时器，以1Hz频率发布数据
        timer_ = this->create_wall_timer(
            1000ms,std::bind(&CelsiusPublisher::timer_callback,this));
        // 初始化随机数生成器
        std::random_device rd;
        gen_ = std::mt19937(rd());
        // 设置温度范围：-273.15°C 到 273.15°C
        dist_ = std::uniform_real_distribution<>(-273.15,273.15);
        RCLCPP_INFO(this->get_logger(),"Temperature publisher node started");
    }
private:
    void timer_callback()
    {
        auto message = std_msgs::msg::Float32();
        // 生成随机温度值
        message.data = dist_(gen_);
        // 发布消息
        publisher_->publish(message);
        // 打印日志
        RCLCPP_INFO(this->get_logger(),"Publisher temperature: %.2f°C",message.data);
        count_++;
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    size_t count_;

    // 随机数生成相关
    std::mt19937 gen_;
    std::uniform_real_distribution<> dist_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<CelsiusPublisher>());
    rclcpp::shutdown();
    return 0;
}