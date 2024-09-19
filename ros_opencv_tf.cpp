#include <opencv2/opencv.hpp>  
#include <rclcpp/rclcpp.hpp>  
#include "tf2_ros/buffer.h"  
#include "tf2_ros/transform_listener.h"  
#include "geometry_msgs/msg/point_stamped.hpp"  
#include "sensor_msgs/msg/image.hpp"  
#include "cv_bridge/cv_bridge.h"  
#include <iostream>  
#include <vector> 
#include <limits>
 
class img_listener : public: rclcpp::Node{
Public:
    img_listener():Node("image_saver"){
        catch_img_=this->create_subscription<sensor_msgs::msg::Image>("image_saver",std::bind(&img_listener::on_timer,this,std::placeholders::_1));
        buffer_=std::make_unique<tf2_ros::Buffer>(this->get_clock());
        listener_=std::make_shared<tf2_ros::TransformListener>(*buffer_,this);
    }

Private:
    void on_timer(const sensor_msgs::msg::Image::SharedPtr img ){
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);
        cv::Mat image=cv_ptr->image;
  
        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        cv::Scalar lower_red = cv::Scalar(0, 120, 70);
        cv::Scalar upper_red = cv::Scalar(10, 255, 255);
        cv::Scalar lower_blue = cv::Scalar(110, 50, 50);
        cv::Scalar upper_blue = cv::Scalar(130, 255, 255);

        cv::Mat mask_red, mask_blue;
        cv::inRange(hsv, lower_red, upper_red, mask_red);
        cv::inRange(hsv, lower_blue, upper_blue, mask_blue);
        
        std::vector<std::vector<cv::Point>> contours_red, contours_blue;
        cv::findContours(mask_red, contours_red, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(mask_blue, contours_blue, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        
        double red_max = 70.0;
        double red_min = 48.0;
        double blue_max = 89.0;
        double blue_min = 75.0;
        vector<cv::Rect>light;

        for (const auto& contour : contours_red) {
            double area = cv::contourArea(contour);
            std::cout << "Red contour area: " << area << std::endl;
            if (area >= red_min && area <= red_max) {
                cv::Rect rect = cv::boundingRect(contour);
                if (rect.width >= rect.height * 2||rect.height>=rect.width*2) {
                    std::cout << "Red Band: (" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << ")" << std::endl;
                    cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 1); 
                    light.push_back(rect);
                }
            }
        }

        

        cv::Point topLeftMost(numeric_limits<int>::max(), numeric_limits<int>::max());
        cv::Point bottomRightMost(numeric_limits<int>::min(), numeric_limits<int>::min());
        for (const auto& rect_ : light) {
            cout << rect_ << endl;
            cv::Point topLeft = rect_.tl();
            cv::Point bottomRight = rect_.br();

        
            topLeftMost.x = min(topLeftMost.x, topLeft.x);
            topLeftMost.y = min(topLeftMost.y, topLeft.y);

            bottomRightMost.x = max(bottomRightMost.x, bottomRight.x);
            bottomRightMost.y = max(bottomRightMost.y, bottomRight.y);
        }
        cv::rectangle(image, cv::Point(topLeftMost.x, topLeftMost.y), cv::Point(bottomRightMost.x, bottomRightMost.y), cv::Scalar(0, 255, 0), 5);

        
        std::vector<Point2d> image_points;
        image_points.push_back(Point2d(topLeftMost.x, topLeftMost.y));
        image_points.push_back(Point2d(bottomRightMost.x, topLeftMost.y));
        image_points.push_back(Point2d(bottomRightMost.x, bottomRightMost.y));
        image_points.push_back(Point2d(topLeftMost.x, bottomRightMost.y));

        


        std::vector<Point3d> model_points;
        model_points.push_back(Point3d(0, 0, 0)); 
        model_points.push_back(Point3d(160, 0, 0));
        model_points.push_back(Point3d(160, 80, 0));
        model_points.push_back(Point3d(0, 80, 0));



        // 相机内参矩阵
        Mat camera_matrix = (Mat_<double>(3, 3) << 1462.3697, 0, 398.59394,
            0, 1469.68385, 110.68997,
            0, 0, 1);
        // 相机畸变系数
        Mat dist_coeffs = (Mat_<double>(5, 1) << 0.003518, -0.311778,
            -0.016581, 0.023682, 0.0000);

        cout << "Camera Matrix " << endl << camera_matrix << endl << endl;
        // 旋转向量
        Mat rotation_vector;
        // 平移向量
        Mat translation_vector;

        // pnp求解
        solvePnP(model_points, image_points, camera_matrix, dist_coeffs, \
            rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);


        cout << "Rotation Vector " << endl << rotation_vector << endl << endl;
        cout << "Translation Vector" << endl << translation_vector << endl << endl;

        Mat Rvec;
        Mat_<float> Tvec;
        rotation_vector.convertTo(Rvec, CV_32F); 
        translation_vector.convertTo(Tvec, CV_32F); 

        Mat_<float> rotMat(3, 3);
        Rodrigues(Rvec, rotMat);

        cout << "rotMat" << endl << rotMat << endl << endl;

        double distance_to_origin = norm(Tvec);
        cout << "Distance from Camera to Origin (in mm): " << distance_to_origin << endl;
        std::vector<Point3f> camera_points;
        for (const auto& pt : model_points) {
            Point3f camera_pt;  
            camera_pt.x = rotMat.at<float>(0, 0) * pt.x + rotMat.at<float>(0, 1) * pt.y + rotMat.at<float>(0, 2) * pt.z + Tvec.at<float>(0);
            camera_pt.y = rotMat.at<float>(1, 0) * pt.x + rotMat.at<float>(1, 1) * pt.y + rotMat.at<float>(1, 2) * pt.z + Tvec.at<float>(1);
            camera_pt.z = rotMat.at<float>(2, 0) * pt.x + rotMat.at<float>(2, 1) * pt.y + rotMat.at<float>(2, 2) * pt.z + Tvec.at<float>(2);
            camera_points.push_back(camera_pt);
        }
        
        for (size_t i = 0; i < camera_points.size(); ++i) {
            std::cout << "Camera Coordinate of Point " << i + 1 << ": (" << camera_points[i].x << ", " << camera_points[i].y << ", " << camera_points[i].z << ")" << std::endl;
        }

        
        cv::imshow("Detected Bands", image);

        geometry_msgs::msg::PointStamped camera_liner;
        geometry_msgs::msg::PointStamped odom_liner; 
        for (int i =0;i<camera_points.size();++i){
            camera_liner.header.stamp = this->get_clock()->now();  
            camera_liner.header.frame_id = "frame_parent";  
            camera_liner.point.x = camera_points[i].x;  
            camera_liner.point.y = camera_points[i].y;  
            camera_liner.point.z = camera_points[i].z; 
            odom_liner.header.stamp = this->get_clock()->now(); 
            odom_liner.header.frame_id = "odom"; 
            try {  
                buffer_->transform(carema_liner, odom_liner, "odom");  
                RCLCPP_INFO(this->get_logger(), "转换后的坐标: (%.2f, %.2f, %.2f)",  
                            odom_liner.point.x, odom_liner.point.y, odom_liner.point.z);  
            } catch (const tf2::TransformException& ex) {  
                RCLCPP_ERROR(this->get_logger(), "%s", ex.what());  
            }   
        }

    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr catch_img_;  
    std::unique_ptr<tf2_ros::Buffer>buffer_;
    std::shared_ptr<tf2_ros::TransformListener>listener_;
            
}

int (argc,char * argv[]){
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<img_listener>());
    rclcpp::shutdown();

    return 0;
}