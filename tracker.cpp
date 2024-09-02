#include "rclcpp/rclcpp.hpp"
#include "tracker_founder_cpp/msg/parameter.hpp"
using tracker_founder_cpp::msg::Parameter;
using namespace std::chrono_literals;
class tracker:public rclcpp::Node{
public:
    tracker():Node("tracker_node_cpp"),count(0){
        publisher_=this->create_publisher<Parameter>("parameter_show",10);
        timer_=this->create_wall_timer(500ms,std::bind(&tracker::on_timer,this));
    }
private:
    void on_timer(){
        auto para=Parameter();
        para.xyz=0.50;
        para.yaw=0.355;
        para.r=0.802;
        para.vx=2;
        para.vy=1;
        para.vz=3;

        publisher_->publish(para);
    RCLCPP_INFO(this->get_logger(),"参数:(%.2f,%.3f,%.3f,%d.%d,%d)",para.xyz,para.yaw,para.r,para.vx,para.vy,para.vz);
    }
    rclcpp::Publisher<Parameter>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count;
};

int main(int argc,char ** argv){
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<tracker>());
    rclcpp::shutdown();

    return 0;
}