#include "ros/ros.h"
#include "std_msgs/String.h"
#include "string"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Pose.h"

int get_int()
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

int get_float()
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
      std::cout << "Test" << std::endl;
      ROS_INFO("Bitte eine Zahl eingeben");
    }
    catch (std::invalid_argument const&)
    {
      ROS_INFO("Bitte eine Zahl eingeben");
    }
  } while (!valid);
  return tmp;
}

int main(int argc, char **argv)
{
  int mode = 0;
  geometry_msgs::Pose pose;
  geometry_msgs::Vector3 winkel;

  ros::init(argc, argv, "user_interface");

  ros::NodeHandle n;

  ros::Publisher set_pose_pub = n.advertise<geometry_msgs::Pose>("set_pose", 1);
  ros::Publisher set_angles_pub = n.advertise<geometry_msgs::Vector3>("set_angles", 1);
  while(1){
    do
    {
      std::cout << "Bitte wählen Sie eine Möglichkeit aus:" << std::endl;
      std::cout << "| 1: TCP Pose eingeben\n| 2: Motoren Winkel eingeben\n| 3: Beenden" << std::endl;

      mode = get_int();

  } while (!(mode == 1 || mode == 2 || mode == 3));


    switch (mode){
      case 1:
        std::cout << "Pose eingeben: x y z\nX: ";
        pose.position.x = get_float();
        std::cout << "Y: ";
        pose.position.y = get_float();
        std::cout << "Z: ";
        pose.position.z = get_float();
        set_pose_pub.publish(pose);
        break;
      case 2:
        std::cout << "Motor Winkel eingeben: (in Grad°)\nMotor1: ";
        winkel.x = get_float();
        std::cout << "Motor2: ";
        winkel.y = get_float();
        std::cout << "Motor3: ";
        winkel.z = get_float();
        set_angles_pub.publish(winkel);
        break;
      case 3:
        return 0;
        break;

      default:
        std::cout << "Wie hast du es geschaft hierher zu kommen??????" << std::endl;
        break;
    }
  }
}
