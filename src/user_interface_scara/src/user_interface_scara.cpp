#include "ros/ros.h"
#include "string"
#include <thread>
#include <chrono>
#include <math.h>


//Old Msgs Types
//#include "std_msgs/String.h"
//#include "geometry_msgs/Vector3.h"
//#include "geometry_msgs/Pose.h"

//new Msgs Types
#include <std_msgs/Float64MultiArray.h>
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"



class UserInterface
{
public:
  UserInterface();
  int get_int();
  float get_float();
  geometry_msgs::Pose pose;
  std_msgs::Float64MultiArray winkel;
  ros::Publisher set_pose_pub;
  ros::Publisher set_angles_pub;
  ros::Subscriber calc_pose_sub;
  ros::Subscriber calc_angles_sub;
private:
  void calc_pose_callback(const geometry_msgs::Pose& pose);
  void calc_angles_callback(const std_msgs::Float64MultiArray& angles_array);
  ros::NodeHandle n;
};

UserInterface::UserInterface()
{
  set_pose_pub = n.advertise<geometry_msgs::Pose>("set_pose", 1);
  set_angles_pub = n.advertise<std_msgs::Float64MultiArray>("set_angles_array", 1);
  calc_pose_sub = n.subscribe("/calc_pose", 100, &UserInterface::calc_pose_callback, this);
  calc_angles_sub = n.subscribe("/calc_angles_array", 100, &UserInterface::calc_angles_callback, this);
  winkel.data.resize(5);
}


int UserInterface::get_int()
{
  std::string input;
  int tmp = 0;
  bool valid = false;
  do
  {
    try
    {
      std::cout << "Auswahl: ";
      std::getline(std::cin, input);
      tmp = std::stoi(input);
      valid = true;
    }
    catch (std::invalid_argument const&)
    {
      std::cout << "Bitte eine Zahl eingeben!" << std::endl;
    }
  } while (!valid);
  return tmp;
}

float UserInterface::get_float()
{
  std::string input;
  float tmp = 0;
  bool valid = false;
  do
  {
    try
    {
      std::getline(std::cin, input);
      tmp = std::stof(input);
      valid = true;
    }
    catch (std::invalid_argument const&)
    {
      ROS_INFO("Bitte eine Zahl eingeben");
    }
  } while (!valid);
  return tmp;
}

void UserInterface::calc_pose_callback(const geometry_msgs::Pose& pose)
{
  std::printf("\nCallculated Pose is: X: %.2f  Y: %.2f  Z: %.2f", pose.position.x,
            pose.position.y, pose.position.z);
}

void UserInterface::calc_angles_callback(const std_msgs::Float64MultiArray& angles_array)
{
  ROS_INFO("Got calculated Angles");
}

void run_ros()
{
  ros::spin();
}

float grad_to_rad(float grad)
{
  float rad = grad * (M_PI/180);
  if (rad > 3.14) rad = -(6.28 - rad);
  std::cout << "rad: " << std::to_string(rad) << std::endl;
  return rad;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "user_interface");
  int mode = 0;
  UserInterface user_interface;
  std::thread t1(run_ros);

  while(1){
    do
    {
      std::cout << "Bitte wählen Sie eine Möglichkeit aus:" << std::endl;
      std::cout << "| 1: TCP Pose eingeben\n| 2: Motoren Winkel eingeben\n| 3: Beenden" << std::endl;

      mode = user_interface.get_int();

    } while (!(mode == 1 || mode == 2 || mode == 3));

    switch (mode){
      case 1:
        std::cout << "Pose eingeben: x y z\nX: ";
        user_interface.pose.position.x = user_interface.get_float();
        std::cout << "Y: ";
        user_interface.pose.position.y = user_interface.get_float();
        std::cout << "Z: ";
        user_interface.pose.position.z = user_interface.get_float();
        user_interface.set_pose_pub.publish(user_interface.pose);
        break;
      case 2:
        std::cout << "Motor Winkel eingeben: (in Grad°)\nMotor1: ";
        user_interface.winkel.data[0] = grad_to_rad(user_interface.get_float());
        std::cout << "Motor2: ";
        user_interface.winkel.data[1] = grad_to_rad(user_interface.get_float());
        std::cout << "Motor3: ";
        user_interface.winkel.data[2] = grad_to_rad(user_interface.get_float());
        user_interface.set_angles_pub.publish(user_interface.winkel);
        break;
      case 3:
        return 0;
        break;

      default:
        std::cout << "Wie hast du es geschaft hierher zu kommen??????" << std::endl;
        break;

    std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
    }
  }
}
