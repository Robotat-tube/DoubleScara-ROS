/*
 * You can add these libraries to the arduino libraries folder to see some examples
 * http://learn.makeblock.com/en/learning-arduino-programming-ranger/
 * 
 * 
 * Use Arduino Mega 2560 board. It is compatible with makeblock Me Auriga
 * 
 */

#include <ros.h>
//  http://www.yarp.it/git-master/classyarp_1_1rosmsg_1_1std__msgs_1_1Int16MultiArray.html
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/String.h>
#include <SoftwareSerial.h> // Makeblock Smart Servo MS-12A library
#include "MeAuriga.h"


MeSmartServo mysmartservo1(PORT1);
MeSmartServo mysmartservo2(PORT2);   
float defaultspeed = 2; //just for testing -- unit: rpm
ros::NodeHandle  nh;

//  http://www.yarp.it/git-master/classyarp_1_1rosmsg_1_1std__msgs_1_1Int16MultiArray.html
void servo_cb( const std_msgs::Int16MultiArray& cmd_msg){
  mysmartservo1.moveTo(PORT1,cmd_msg.data[0], defaultspeed);
  mysmartservo2.moveTo(PORT2,cmd_msg.data[1], defaultspeed);
  Serial.println("servo_cb received the following: ");
  Serial.println("Data[0]: " + String(cmd_msg.data[0]));
  Serial.println("Data[1]: " + String(cmd_msg.data[1]));
  
}


ros::Subscriber<std_msgs::Int16MultiArray> sub("position_pub", servo_cb);
std_msgs::String str_msg;
ros::Publisher megaChatter("megaChatter", &str_msg);


void setup(){
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(megaChatter);
  nh.getHardware()->setBaud(57600);
  
  Serial.begin(57600);
  mysmartservo1.begin(57600);
  delay(5);
  mysmartservo1.assignDevIdRequest();
  delay(50);
  Serial.println("mysmartservo1 setup!");


  mysmartservo2.begin(57600);
  delay(5);
  mysmartservo2.assignDevIdRequest();
  delay(50);
  Serial.println("mysmartservo2 setup!");
  
  delay(2000);
}

void loop(){
  str_msg.data = "mega chatter says hello";
  megaChatter.publish( &str_msg );
  nh.spinOnce();
  delay(100);
}
