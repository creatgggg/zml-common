#include "rclcpp/rclcpp.hpp"
#include "tracker_founder_cpp/msg/parameter.hpp"
using tracker_founder_cpp::msg::Parameter;
class founder:public rclcpp::Node{
public:
    founder():Node("founder_node_cpp"){
        sub_=this->create_subscription<Parameter>("parameter_show",10,std::bind(&founder::do_callback,this,std::placeholders::_1));
    }
private:
    void do_callback(const Parameter &para){
        RCLCPP_INFO(this->get_logger(),"订阅的信息:xyz=%.2f,yaw=%.2f,r=%.2f,vx=%.d,vy=%.d,vz=%.d",para.xyz,para.yaw,para.r,para.vx,para.vy,para.vz);
    }
    rclcpp::Subscription<Parameter>::SharedPtr sub_;
};
int main(int argc,char**argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<founder>());
    rclcpp::shutdown();
    return 0;
}