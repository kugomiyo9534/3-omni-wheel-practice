#include <ros/ros.h>
#include <std_msgs/Int16MultiArray.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Int16.h>

#define MOTOR_NUM 3
#define PW_MAX 100 //モータ出力絶対値の最大値
#define SQRT2 1.41421356
#define SQRT3 1.7320504
#define DIRE 1 //モータ回転方向調整用の変数

class OmniController {
private:
  ros::Publisher pub;
  ros::Publisher pub2;
  ros::Subscriber sub;
  double vx, vy, ang;
  double v_raw[MOTOR_NUM];
  int16_t v[MOTOR_NUM];
  std_msgs::Int16MultiArray order;
  std_msgs::Int16 log;
  
public:
  OmniController(ros::NodeHandle& nh) : vx(0), vy(0), ang(0) {
    pub = nh.advertise<std_msgs::Int16MultiArray>("omni",1);
    pub2 = nh.advertise<std_msgs::Int16>("log",1);
    sub = nh.subscribe<sensor_msgs::Joy>("joy", 1, &OmniController::callback, this);
  }
  
  void callback(const sensor_msgs::Joy::ConstPtr& joy_msg) {
    //-1.0 <= vx,vy <= 1.0
    vx = joy_msg->axes[2];
    vy = joy_msg->axes[3];
    ang = joy_msg->axes[0];
    //ROS_INFO("log:%f", vx);
    log.data = vx;
    pub2.publish(log);
  }
​
  void omni_calc() {
    //位置:回転=1:sqrt2 → 1:1 にスケーリング
    //ang *= 0.5*SQRT2;
​
    v_raw[0] =         vx                + ang;
    v_raw[1] =   - 0.5*vx - 0.5*SQRT3*vy + ang;
    v_raw[2] =   - 0.5*vx + 0.5*SQRT3*vy + ang;
​
    /*v_raw[0] = - vx - vy + ang;
    v_raw[1] =   vx - vy + ang;
    v_raw[2] =   vx + vy + ang;
    v_raw[3] = - vx + vy + ang;*/
​
    //PW_MAX
    for (int i = 0; i < MOTOR_NUM; i++) {
      v[i] = v_raw[i] * PW_MAX / 3.0;
    }
  }
​
  void publish() {
    order.data.resize(3);
    //order.data.assign(MOTOR_NUM, 0);
    for (int i = 0; i < MOTOR_NUM; i++) {
    order.data[i] = v[i];
    }
    pub.publish(order);
  }
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "omni_3_calcullater");
  ros::NodeHandle nh;
  OmniController omniController(nh);
  ros::Rate loop_rate(10);
    
  while(ros::ok()) {
    ros::spinOnce();
    omniController.omni_calc();
    omniController.publish();
    loop_rate.sleep();
    }
}
