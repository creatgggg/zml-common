#include <opencv2/opencv.hpp>  
#include <rclcpp/rclcpp.hpp>  
#include "sensor_msgs/msg/image.hpp"  
#include "cv_bridge/cv_bridge.h"  
  
class image_sender : public rclcpp::Node {  
public:  
    image_sender() : Node("image_publisher") {  
        publisher_ = this->create_publisher<sensor_msgs::msg::Image>("image_saver", 10);    
        cv::Mat image = cv::imread("home/fairy/image.jpg");  
        sensor_msgs::msg::Image ros_image;  
        cv_bridge::CvImage bridge(std_msgs::msg::Header(), "bgr8", image);  
        bridge.toImageMsg(ros_image);    
        RCLCPP_INFO(this->get_logger(), "Publishing image...");  
        publisher_->publish(ros_image);  
    }  
  
private:  
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;  
};  
  
int main(int argc, char **argv) {  
    rclcpp::init(argc, argv);  
    rclcpp::spin(std::make_shared<Image_sender>());  
    rclcpp::shutdown();  
    return 0;  
}