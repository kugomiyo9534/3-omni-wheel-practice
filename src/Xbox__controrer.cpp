#include <ros/ros.h>
//#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

/*
axes[0] : 左ジョイ 左(1.0) 右(-1.0)
axes[1] : 左ジョイ 上(1.0) 下(-1.0)
axes[2] : 右ジョイ 左(1.0) 右(-1.0)
axes[3] : 右ジョイ 上(1.0) 下(-1.0)

buttons[0] : A
buttons[1] : B
buttons[2] : X
buttons[3] : Y
buttons[4] : LB
buttons[5] : RB
buttons[6] : LT
buttons[7] : RT
buttons[8] : BACK
buttons[9] : START
buttons[10] : HOME(中央)
buttons[11] : 左ジョイ押し込み
buttons[12] : 右ジョイ押し込み
buttons[13] : 十字 左
buttons[14] : 十字 右
buttons[15] : 十字 上
buttons[16] : 十字 下
*/


void joy_callback(const sensor_msgs::Joy &joy_msg)
{
  // 処理内容を記述
  ROS_INFO("axes[2]:%f", joy_msg.axes[2]);
  ROS_INFO("axes[3]:%f", joy_msg.axes[3]);   // スティック0の状態を表示 (-1 ～ 1)
  ROS_INFO("Button[4]:%d", joy_msg.buttons[4]);
  ROS_INFO("Button[5]:%d", joy_msg.buttons[5]);  // ボタン0の状態を表示 (0 or 1)
}

int main(int argc, char **argv) //Publisher
{
  ros::init(argc, argv, "joy_sub_node");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("joy", 10, joy_callback);
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}