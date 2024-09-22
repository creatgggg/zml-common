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
    Robot_A(std::string path) 
        : Node("robot_a"),
          enemy_detected_pub_(this->create_publisher<robot_assist_cpp::msg::Detect>("detect_topic", 10)),
          video_path_(path),
          video(video_path_), 
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

        
        cvtColor(frame, binary, COLOR_BGR2GRAY);  
        threshold(binary, binary, kThreshold, kMaxVal, THRESH_BINARY);  
        GaussianBlur(binary, Gaussian, kGaussianBlurSize, 0);  

        
        findContours(Gaussian, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  

        
        detectedRects.clear();  
        for (const auto& contour : contours) {  
            Rect boundRect = boundingRect(contour);  
            if (boundRect.height / static_cast<double>(boundRect.width) >= 1.3 &&  
                boundRect.height > 36 && boundRect.width > 20) {  
                detectedRects.push_back(boundRect);  
            }  
        }

        for (const auto& rect : detectedRects) {
            Point tl_ = rect.tl();
            Point br_ = rect.br();
            rectangle(frame, tl_, br_, Scalar(0, 255, 0), 1);
        }  
        

        if (!detectedRects.empty()) {  
            auto msg = std::make_shared<robot_assist_cpp::msg::Detect>();  
            msg->detected = true;  
            msg->x = detectedRects[0].x;  
            msg->y = detectedRects[0].y;  
            msg->z = 0.0;    
            enemy_detected_pub_->publish(*msg);  
        }
        imshow("detected",frame);
        waitKey(1); 
    }

    rclcpp::Publisher<robot_assist_cpp::msg::Detect>::SharedPtr enemy_detected_pub_; 
    std::string video_path_;
    VideoCapture video;
    rclcpp::TimerBase::SharedPtr timer_; 
};  

int main(int argc, char **argv) {  
    rclcpp::init(argc, argv);
    if (argc < 2) {  
        std::cerr << "Usage: " << argv[0] << " <video_file_path>" << std::endl;  
        return 1;  
    }  

    std::string path = argv[1];    
    rclcpp::spin(std::make_shared<Robot_A>(path));  
    rclcpp::shutdown();  
    return 0;  
} 
