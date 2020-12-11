#include "ros/ros.h"
#include <geometry_msgs/Pose.h>
#include <std_msgs/Float64MultiArray.h>
#include <math.h>

float degreetorad(float degree){
    return (degree * (M_PI/180));
}

float radtodegree(float rad){
    return (rad * (180/M_PI));
}


class Forward_Kinematics_Class{
public:
  Forward_Kinematics_Class()
  {
    //Topic you want to publish to
    pub_ = nh_.advertise<geometry_msgs::Pose>("/calc_pose", 1);

    //Topic you want to subscribe to
    sub_ = nh_.subscribe("/set_angles_array", 1000, &Forward_Kinematics_Class::callback_subscriber,this);

    //PARAMETER AUSLESEN UND GEOMETRIE ZUWEISEN!!!!!
	L0 = 0.114; //in Meter
	L1 = 0.06;
	L2 = 0.06;
	z_motor_radius = 0.014;
  }

   float angleToLine(float z_motor_angle){
      return( degreetorad(z_motor_angle)*z_motor_radius );
   }

  void callback_subscriber(const std_msgs::Float64MultiArray& sub_msg){ 
    //sub_msg verarbeiten und leftMotor, rightMotor auslesen
    float leftMotor = sub_msg.data[0];
    float rightMotor = sub_msg.data[1];
    float zMotor = sub_msg.data[2];


    float xa, ya, xb, yb, h, phi, delta, xc, yc, left_arm_angle, right_arm_angle;
    xa = -L1*sin(degreetorad(leftMotor)); // left motor
    ya = L1*cos(degreetorad(leftMotor));
    xb = L1*sin(degreetorad(rightMotor)) + L0; // right motor
    yb = L1*cos(degreetorad(rightMotor));

    h = sqrt(pow(xb - xa, 2) + pow(yb - ya, 2));
    phi = atan2((yb - ya),(xb - xa));
    delta = acos(h/(2*L2));
    xc = xa + L2*cos(phi + delta);
    yc = ya + L2*sin(phi + delta);

    // left_arm_angle = radtodegree( atan2((yc - ya),(xc - xa)) );
    // right_arm_angle = -radtodegree( atan2((yc - yb),(xc - xb)) ); // multiplied with minus -> anticlockwise
    //pass calculated coordinates in msg
    pub_pos_msg.position.x = xc;
    pub_pos_msg.position.y = yc;
    pub_pos_msg.position.z = angleToLine(zMotor);	

    pub_.publish(pub_pos_msg);
  } //Callback-Ende

protected:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
  geometry_msgs::Pose pub_pos_msg;

  //Arme:
  float L0, L1, L2, z_motor_radius;

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
