#include"rclcpp/rclcpp.hpp"
#include"tf2_ros/static_transform_broadcaster.h"
#include"geometry_msgs/msg/transform_stamped.hpp"
#include"tf2/LinearMath/Quaternion.h"
class tf_broadcaster:public rclcpp::Node{
public:
    tf_broadcaster(Char*argv):Node("tf_broadcaster"){
    broadcaster_=std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
    pub_tf(argv);   
    }
private:
   std::shared_ptr<tf2_ros::StaticTransformBroadcaster>broadcaster_;
   void pub_tf(Char*argv[]){
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp=this->now();
    transform.header.frame_id=argv[7];
    transform.header.child_frame_id=argv[8];
    transform.transform.translation.x=argv[1];
    transform.transform.translation.x=argv[2];
    transform.transform.translation.x=argv[3];
    tf2::Quaternion qtn;
    qtn.setRPY(atof(argv[4]),atof(argv[5],atof(argv[6])));
    transform.transform.rotation.x=qtn.x();
    transform.transform.rotation.y=qtn.y();
    transform.transform.rotation.z=qtn.z();
    transform.transform.rotation.w=qtn.w();
    broadcaster_->sendTransform(transform);
   }
};

int main(int argc,char * argv[]){
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<tf_broadcster>(argv));
    rclcpp::shutdown();

    return 0;
}