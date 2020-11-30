#include "ros/ros.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Vector3.h>
#include <math.h>

class Forward_Kinematics_Class{
public:
  Forward_Kinematics_Class()
  {
    //Topic you want to publish to
    pub_ = nh_.advertise<geometry_msgs::Pose>("/calc_pose", 1);

    //Topic you want to subscribe to
    sub_ = nh_.subscribe("/set_angles", 1000, &Forward_Kinematics_Class::callback_subscriber,this);

    //PARAMETER AUSLESEN UND GEOMETRIE ZUWEISEN!!!!!
  	xo1 = -10;
  	yo1 = 0;
  	xo2 = 10;
  	yo2 = 0;
  	L1 = 10;
  	L2 = 10;
  	L3 = 10;
  	L4 = 10;
  }

  void callback_subscriber(const geometry_msgs::Vector3& sub_msg){
    //sub_msg verarbeiten und leftMotor, rightMotor auslesen
    float leftMotor = sub_msg.x;
    float rightMotor = sub_msg.y;

    float xa, ya, xb, yb, h, phi, delta, xc, yc;
    xa = xo1 + L2*cos(leftMotor*M_PI/180); // left motor
    ya = yo1 + L2*sin(leftMotor*M_PI/180);
    xb = xo2 + L1*cos(rightMotor*M_PI/180); // right motor
    yb = yo2 + L1*sin(rightMotor*M_PI/180);

    h = sqrt(pow(xb - xa, 2) + pow(yb - ya, 2));
    phi = atan2((yb - ya),(xb - xa));
    delta = acos(h/(2*L4));  // Assuming L3 = L4
    xc = xa + L4*cos(phi + delta);
    yc = ya + L4*sin(phi + delta);

    //pass calculated coordinates in msg
    pub_pos_msg.position.x = xc;
    pub_pos_msg.position.y = xc;

    pub_.publish(pub_pos_msg);
  } //Callback-Ende

protected:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
  geometry_msgs::Pose pub_pos_msg;

  //Arme:
  float L1, L2;
  float L3, L4;
  //Base:
  float xo1, xo2, yo1, yo2;
};//End of class Forward_Kinematics_Class

//-----------------------//
//--- Main processing ---//
//-----------------------//
/// Main Function
/** Responsible for argument evaluation, Subscriber Handling/Synchronization and ros::spin() */
int main(int argc, char **argv){

  ros::init(argc, argv, "forward_kinematics");
  //Create the Object
  Forward_Kinematics_Class forward_double_scara;
  ros::spin();
  //everything is fine :)
}
