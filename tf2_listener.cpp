#include"rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include"tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/point_stamped.hpp"
using namespace std::chrono_literals;
geometry_msgs::msg::PointStamped cinema_liner;
geometry_msgs::msg::PointStamped odom_liner; 
class tf_listener:public rclcpp::Node{
public:
    tf_listener():Node("tf_listener"){
        buffer_=std::make_unique<tf2_ros::Buffer>(this->get_clock());
        listener_=std::make_shared<tf2_ros::TransformListener>(*buffer_,this);
        cinema_liner.header.stamp = this->get_clock()->now();  
        cinema_liner.header.frame_id = "frame_parent";  
        cinema_liner.point.x = 1.0;  
        cinema_liner.point.y = 2.0;  
        cinema_liner.point.z = 3.0;  
        timer_=this->create_wall_timer(1s,std::bind(&tf_listener::on_timer,this));
    }
private:
    void on_timer(){
        odom_liner.header.stamp = this->get_clock()->now(); 
        odom_liner.header.frame_id = "odom"; 
        try {  
            buffer_->transform(cinema_liner, odom_liner, "odom");  
            RCLCPP_INFO(this->get_logger(), "转换后的坐标: (%.2f, %.2f, %.2f)",  
                        odom_liner.point.x, odom_liner.point.y, odom_liner.point.z);  
        } catch (const tf2::TransformException& ex) {  
            RCLCPP_ERROR(this->get_logger(), "%s", ex.what());  
        }  
    }
    std::unique_ptr<tf2_ros::Buffer>buffer_;
    std::shared_ptr<tf2_ros::TransformListener>listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc,char * argv[]){
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<tf_listener>());
    rclcpp::shutdown();

    return 0;
}