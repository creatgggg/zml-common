#include "rclcpp/rclcpp.hpp"
#include "robot_assist_cpp/msg/detect.hpp"
  
class Robot_B : public rclcpp::Node {  
public:  
    Robot_B() : Node("robot_b"), count_(0) {  
        enemy_detected_sub_ = this->create_subscription<robot_assist_cpp::msg::Detect>(  
            "detect_topic", 10,  
            std::bind(&Robot_B::enemy_detected_callback, this, std::placeholders::_1));  
  
        // 设置一个定时器来模拟到达或撤退（仅作为示例）  
        timer_ = this->create_wall_timer(  
            std::chrono::seconds(30),  
            std::bind(&Robot_B::check_arrival, this));  
    }  
  
private:  
    void enemy_detected_callback(const std::shared_ptr<robot_assist_cpp::msg::Detect> msg) {  
        RCLCPP_INFO(this->get_logger(), "Enemy detected at (%f, %f,%f). Heading to A's location.",  
                    msg->x, msg->y,msg->z);  
    }  
  
    void check_arrival() {  
        count_++;  
        if (count_ == 1) {  // 假设第一次回调时未到达  
            RCLCPP_INFO(this->get_logger(), "Failed to reach A's location within 30 seconds. Retreating.");  
            // 在这里可以发送撤退信号（如果定义了相应的消息和发布者）  
        }  
    }  
  
    rclcpp::Subscription<robot_assist_cpp::msg::Detect>::SharedPtr enemy_detected_sub_;  
    rclcpp::TimerBase::SharedPtr timer_;  
    int count_;  
};  
  
int main(int argc, char **argv) {  
    rclcpp::init(argc, argv);  
    rclcpp::spin(std::make_shared<Robot_B>());  
    rclcpp::shutdown();  
    return 0;  
}