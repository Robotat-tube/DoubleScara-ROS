#include "ros/ros.h"
#include "std_msgs/String.h"
#include "string"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Vector3.h"
#include "sensor_msgs/JointState.h"

class RobotPublish
{
public:
  RobotPublish()
  {
    set_pose_sub = n.subscribe("/set_pose", 1000, &RobotPublish::set_pose_callback, this);
    set_angles_sub = n.subscribe("/set_angles", 1000, &RobotPublish::set_angles_callback, this);
    calc_pose_sub = n.subscribe("/calc_pose", 1000, &RobotPublish::calc_pose_callback, this);
    calc_angles_sub = n.subscribe("/calc_angles", 1000, &RobotPublish::calc_angles_callback, this);
    pup_angles_pup  = n.advertise<sensor_msgs::JointState>("pup_angles", 100);
    state.position.resize(5);
    state.name.resize(5);
    state.name[0] = "Left_Shoulder";
    state.name[1] = "Left_Elbow";
    state.name[2] = "Right_Shoulder";
    state.name[3] = "Right_Elbow";
    state.name[4] = "Z";
  }
private:
  ros::NodeHandle n;
  ros::Subscriber set_pose_sub;
  ros::Subscriber set_angles_sub;
  ros::Subscriber calc_pose_sub;
  ros::Subscriber calc_angles_sub;
  ros::Publisher pup_angles_pup;
  sensor_msgs::JointState state;
  geometry_msgs::Pose current_pose;
  geometry_msgs::Vector3 current_angles;
  void set_pose_callback(const geometry_msgs::Pose &pose);
  void set_angles_callback(const geometry_msgs::Vector3 &angles);
  void calc_pose_callback(const geometry_msgs::Pose &pose);
  void calc_angles_callback(const geometry_msgs::Vector3 &angles);
  float grad_to_rad(float grad);
};

void RobotPublish::set_pose_callback(const geometry_msgs::Pose &pose)
{
  current_pose.position.x = pose.position.x;
  current_pose.position.y = pose.position.y;
  current_pose.position.z = pose.position.z;
}

void RobotPublish::set_angles_callback(const geometry_msgs::Vector3 &angles)
{
  current_angles.x = angles.x;
  current_angles.y = angles.y;
  current_angles.z = angles.z;
  // TODO calc angles for imaginary motors an robot elbows and publish to pup_angles
  state.position[0] = current_angles.x;
  state.position[2] = current_angles.y;
  state.position[4] = current_angles.z;
  pup_angles_pup.publish(state);
}

void RobotPublish::calc_pose_callback(const geometry_msgs::Pose &pose)
{
  current_pose.position.x = pose.position.x;
  current_pose.position.y = pose.position.y;
  current_pose.position.z = pose.position.z;
}

void RobotPublish::calc_angles_callback(const geometry_msgs::Vector3 &angles)
{
  current_angles.x = grad_to_rad(angles.x);
  current_angles.y = grad_to_rad(angles.y);
  current_angles.z = grad_to_rad(angles.z);
  // TODO calc angles for imaginary motors an robot elbows and publish to pup_angles
  state.position[0] = current_angles.y;
  state.position[2] = current_angles.y;
  state.position[4] = current_angles.z;
  pup_angles_pup.publish(state);
}

float RobotPublish::grad_to_rad(float grad)
{
  float rad = grad * (M_PI/180);
  if (rad > 3.14) rad = -(6.28 - rad);
  std::cout << "rad: " << std::to_string(rad) << std::endl;
  return rad;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "robot_publish");
  RobotPublish robot_publish;
  ros::spin();
}
