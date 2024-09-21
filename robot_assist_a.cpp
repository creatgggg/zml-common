#include <rclcpp/rclcpp.hpp>  
#include <rclcpp/timer.hpp>  
#include "robot_assist_cpp/msg/detect.hpp"  
#include <chrono>  
#include <iostream>  
#include <opencv2/opencv.hpp>  
#include <vector>  

using namespace cv;  

const int kThreshold = 220;  
const int kMaxVal = 255;  
const Size kGaussianBlurSize = Size(5, 5);  

class Robot_A : public rclcpp::Node {  
public:  
    Robot_A() 
        : Node("robot_a"), 
          video(0), 
          enemy_detected_pub_(this->create_publisher<robot_assist_cpp::msg::Detect>("detect_topic", 10)),  
          timer_(this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&Robot_A::on_timer_, this))) 
    {
        if (!video.isOpened()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open video stream.");
        }
    }

private:
    void on_timer_() {  
        Mat frame, binary, Gaussian;  
        std::vector<std::vector<Point>> contours;  
        std::vector<Rect> detectedRects;  

        if (!video.read(frame)) {  
            RCLCPP_ERROR(this->get_logger(), "Failed to read frame from video.");  
            return;  
        }  

        // Preprocessing  
        cvtColor(frame, binary, COLOR_BGR2GRAY);  
        threshold(binary, binary, kThreshold, kMaxVal, THRESH_BINARY);  
        GaussianBlur(binary, Gaussian, kGaussianBlurSize, 0);  

        // Find contours  
        findContours(Gaussian, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  

        // Detect rectangles  
        detectedRects.clear();  
        for (const auto& contour : contours) {  
            Rect boundRect = boundingRect(contour);  
            if (boundRect.height / static_cast<double>(boundRect.width) >= 1.3 &&  
                boundRect.height > 36 && boundRect.width > 20) {  
                detectedRects.push_back(boundRect);  
            }  
        }  

        if (!detectedRects.empty()) {  
            auto msg = std::make_shared<robot_assist_cpp::msg::Detect>();  
            msg->detected = true;  
            // Set x, y, z based on the detected rects
            msg->x = detectedRects[0].x;  // Example: using the first detected rectangle
            msg->y = detectedRects[0].y;  
            msg->z = 0.0;  // Set z to a constant value or based on your needs  
            enemy_detected_pub_->publish(*msg);  
        }
    }

    rclcpp::Publisher<robot_assist_cpp::msg::Detect>::SharedPtr enemy_detected_pub_;  
    VideoCapture video;  
    rclcpp::TimerBase::SharedPtr timer_; 
};  

int main(int argc, char **argv) {  
    rclcpp::init(argc, argv);  
    rclcpp::spin(std::make_shared<Robot_A>());  
    rclcpp::shutdown();  
    return 0;  
}  
