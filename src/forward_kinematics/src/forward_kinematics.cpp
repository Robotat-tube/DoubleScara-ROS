#include "ros/ros.h"
#include <geometry_msgs/Pose.h>
#include <std_msgs/Float64MultiArray.h>
#include <math.h>

float degreetorad(float degree)
{
  return (degree * (M_PI / 180));
}

float radtodegree(float rad)
{
  return (rad * (180 / M_PI));
}

class Forward_Kinematics_Class
{
protected:
  ros::NodeHandle nh_;
  ros::Publisher pub_pose_;
  ros::Publisher pub_angle_;
  ros::Subscriber sub_;
  geometry_msgs::Pose pub_pos_msg_;
  std_msgs::Float64MultiArray pub_angle_msg_;

  //Arme:
  float L0_, L1_, L2_, z_motor_radius_;

public:
  Forward_Kinematics_Class()
  {
    //Topic you want to publish to
    pub_pose_ = nh_.advertise<geometry_msgs::Pose>("/calc_pose", 1);
    pub_angle_ = nh_.advertise<std_msgs::Float64MultiArray>("/calc_angles_array", 1);

    //Topic you want to subscribe to
    sub_ = nh_.subscribe("/set_angles_array", 1000, &Forward_Kinematics_Class::callback_subscriber, this);

    //PARAMETER AUSLESEN UND GEOMETRIE ZUWEISEN!!!!!
    L0_ = 0.114; //in Meter
    L1_ = 0.06;
    L2_ = 0.06;
    z_motor_radius_ = 0.014;
  }

  float angleToLine(float z_motor_angle)
  {
    return (degreetorad(z_motor_angle) * z_motor_radius_);
  }

  void callback_subscriber(const std_msgs::Float64MultiArray &sub_msg)
  {
    //sub_msg verarbeiten und leftMotor, rightMotor auslesen
    float leftMotor = sub_msg.data[0];
    float rightMotor = sub_msg.data[1];
    float zMotor = sub_msg.data[2];

    float xa, ya, xb, yb, h, phi, delta, xc, yc, left_arm_angle, right_arm_angle;
    xa = -L1_ * sin(leftMotor); // left motor
    ya = L1_ * cos(leftMotor);
    xb = L1_ * sin(rightMotor) + L0_; // right motor
    yb = L1_ * cos(rightMotor);

    h = sqrt(pow(xb - xa, 2) + pow(yb - ya, 2));
    phi = atan2((yb - ya), (xb - xa));
    delta = acos(h / (2 * L2_));
    xc = xa + L2_ * cos(phi + delta);
    yc = ya + L2_ * sin(phi + delta);

    // left_arm_angle = radtodegree( atan2((yc - ya),(xc - xa)) );
    // right_arm_angle = -radtodegree( atan2((yc - yb),(xc - xb)) ); // multiplied with minus -> anticlockwise
    //pass calculated coordinates in msg
    pub_pos_msg_.position.x = xc;
    pub_pos_msg_.position.y = yc;
    pub_pos_msg_.position.z = angleToLine(zMotor);
    ROS_INFO("forward_kinamatics.cpp: xc= %f, yc=%f, z=%f \n", xc, yc, pub_pos_msg_.position.z);
    pub_pose_.publish(pub_pos_msg_);

    if(!isnan(xc) && !isnan(yc)) {
      //pass calculated angles in msg
      float l04 = calc_l04(xc, yc);
      float l14 = calc_l14(xc, yc);
      float theta3 = calc_theta3(l04, l14);
      float theta4 = calc_theta4(l04, l14);
      pub_angle_msg_.data.resize(5);
      pub_angle_msg_.data[0] = leftMotor;  //XY-Ebene
      pub_angle_msg_.data[1] = rightMotor; //XY-Ebene
      pub_angle_msg_.data[2] = zMotor;     //Z-Achse
      pub_angle_msg_.data[3] = degreetorad(theta3);     //Winkel für simulation
      pub_angle_msg_.data[4] = degreetorad(theta4);     //Winkel für simulation
      pub_angle_.publish(pub_angle_msg_);
    } else {
      ROS_INFO("forward_kinamatics.cpp: Entered angles are not valid!");
    }

  } //Callback-Ende

  float calc_l04(float x, float y)
  {
    return sqrt((x * x) + (y * y));
  }

  float calc_l14(float x, float y)
  {
    return sqrt((L0_ - x) * (L0_ - x) + y * y);
  }

  float calc_theta3(float l04, float l14)
  {
    //Ermittelt ueber den Kosinussatz
    return (180 - radtodegree(acos((L1_ * L1_ + L2_ * L2_ - l04 * l04) / (2 * L1_ * L2_))));
  }

  float calc_theta4(float l04, float l14)
  {
    //Ermittelt ueber den Kosinussatz
    return (180 - radtodegree(acos((L1_ * L1_ + L2_ * L2_ - l14 * l14) / (2 * L1_ * L2_))));
  }

}; //End of class Forward_Kinematics_Class

//-----------------------//
//--- Main processing ---//
//-----------------------//
/// Main Function
/** Responsible for argument evaluation, Subscriber Handling/Synchronization and ros::spin() */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "forward_kinematics");
  //Create the Object
  Forward_Kinematics_Class forward_double_scara;
  ros::spin();
  //everything is fine :)
}
