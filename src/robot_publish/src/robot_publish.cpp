#include "ros/ros.h"
#include "std_msgs/String.h"
#include "string"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/JointState.h"

class RobotPublish
{
public:
  RobotPublish()
  {
    checked_angles_sub = n.subscribe("/checked_angles_array", 1000, &RobotPublish::checked_angles_callback, this);
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
  ros::Subscriber checked_angles_sub;
  ros::Publisher pup_angles_pup;
  sensor_msgs::JointState state;
  geometry_msgs::Pose current_pose;
  geometry_msgs::Vector3 current_angles;
  void checked_angles_callback(const std_msgs::Float64MultiArray &angles);
  float grad_to_rad(float grad);
};


void RobotPublish::checked_angles_callback(const std_msgs::Float64MultiArray &angles)
{
  current_angles.x = angles.data[0];
  current_angles.y = angles.data[1];
  current_angles.z = angles.data[2];
  // TODO calc angles for imaginary motors an robot elbows and publish to pup_angles
  state.position[0] = angles.data[0];
  state.position[1] = angles.data[3];
  state.position[2] = angles.data[1];
  state.position[3] = angles.data[4];
  state.position[4] = angles.data[2];
  pup_angles_pup.publish(state);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "robot_publish");
  RobotPublish robot_publish;
  ros::spin();
}
