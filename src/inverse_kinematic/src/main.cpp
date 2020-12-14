
#include "ros/ros.h"
#include <geometry_msgs/Pose.h>
#include <std_msgs/Float64MultiArray.h>
#include <cmath>

float degreetorad(float degree){
    return (degree * (M_PI/180));
}

float radtodegree(float rad){
    return (rad * (180/M_PI));
}


class Inverse_Class{
    public:
      Inverse_Class()
      {
        //Topic you want to publish
        pub_ = nh_.advertise<std_msgs::Float64MultiArray>("/calc_angles_array", 1);

        //Topic you want to subscribe
        sub_ = nh_.subscribe("/set_pose", 1000, &Inverse_Class::callback,this);

        //PARAMETER AUSLESEN UND GEOMETRIE ZUWEISEN!!!!!
        //Arme:
        L1_ = 0.06; //in Meter
      	L2_ = 0.06; //in Meter
      	//L3_ = 10;
      	//L4_ = 10;
        //Base:
        L0_ = 0.114; //in Meter
        //Circle aprox of gear on Z-Axis
        r_ = 0.014; //in Meter
      }

      void callback(const geometry_msgs::Pose::ConstPtr& sub_msg){
            //BUGFIXING: - atan() returns rad in range [-pi/2,+pi/2] -> OK FUER UNS?????
            //           - acos() returns rad in range [-1,+1] -> OK FUER UNS?????
            //           - Funktionsreihenfolge beachten!
            //ACHSEN KOORDINATEN 0|0 AUFPASSEN! LIEGT DERZEIT IN MOTOR-ACHSE Links
            //es wird Tangens verwendet und kein atan2 - ist das ok???? Kommen da die fehler her?

          //sub_msg verarbeiten und Ex_ Ey_ Ez_ auslesen

          Ex_ = sub_msg->position.x;
          Ey_ = sub_msg->position.y;
          Ez_ = sub_msg->position.z;

          //Equation 3-3
            ROS_INFO("**inverse_kinamatics (all in degree)**");
            ROS_INFO("Ex_= %f \n", Ex_);
            ROS_INFO("Ey_= %f \n", Ey_);
            ROS_INFO("Ez_= %f \n", Ez_);

            l04_    = calc_l04();
            ROS_INFO("l04_= %f \n", l04_);
            alpha_  = calc_alpha();
            ROS_INFO("alpha_= %f \n", alpha_);
            beta_   = calc_beta();
            ROS_INFO("beta_= %f \n", beta_);
            theta0_ = calc_theta0();
            ROS_INFO("theta0_= %f \n", theta0_);


          //Equation 3-4
            l14_    = calc_l14();
            ROS_INFO("l14_= %f \n", l14_);
            gamma_  = calc_gamma();
            ROS_INFO("gamma_= %f \n", gamma_);
            delta_  = calc_delta();
            ROS_INFO("delta_= %f \n", delta_);
            theta1_ = calc_theta1();
            ROS_INFO("theta1_= %f \n", theta1_);

          // Z-Axis
          // Circumference -> 2*PI*r

            theta2_ = lintoangle();
            ROS_INFO("theta2_= %f \n", theta2_);


          //Simulationswinkel:
            theta3_ = calc_theta3();
            ROS_INFO("theta3_= %f \n", theta3_);
            theta4_ = calc_theta4();
            ROS_INFO("theta4_= %f \n", theta4_);


          //pass calculated angles in msg
            pub_msgs_.data.resize(5);
            pub_msgs_.data[0] = degreetorad(theta0_); //XY-Ebene
            pub_msgs_.data[1] = degreetorad(theta1_); //XY-Ebene
            pub_msgs_.data[2] = degreetorad(theta2_); //Z-Achse
            pub_msgs_.data[3] = degreetorad(theta3_); //Winkel für simulation
            pub_msgs_.data[4] = degreetorad(theta4_); //Winkel für simulation

            pub_.publish(pub_msgs_);
      } //Callback-Ende


    float calc_l04(){
        return sqrt((Ex_ * Ex_)*(Ey_ * Ey_));
    }
    float calc_alpha(){
        return (90 - radtodegree(atan2(Ex_, Ey_)));
        //return (90 - radtodegree(atan(Ey_/Ex_)));
    }
    float calc_beta(){
        return radtodegree( acos(  (L1_*L1_ - L2_*L2_ + l04_*l04_)/(2*L1_*l04_) ));
    }
    float calc_theta0(){
        return (beta_ - alpha_);
    }




    float calc_l14(){
        return sqrt( ((L0_-Ex_)*(L0_-Ex_)) + (Ey_*Ey_));
    }
    float calc_gamma(){
        return ( 90 - radtodegree(atan2((L0_ - Ex_),Ey_)));
        //return ( 90 - radtodegree(atan(Ey_/(L0_ - Ex_))));
    }
    float calc_delta(){
        float tmp =  (L1_*L1_ - L2_*L2_ + l14_*l14_)/(2*L1_*l14_);

          //UEberrotationsausgleich - BUG-FIXING: Die rechnung stimmt vielleicht nicht  
        do{
          if(tmp>1){
            tmp = tmp * -1;
            tmp++; 
          }
        }while(tmp>1);

        return ( radtodegree( acos(tmp)));
    }
    float calc_theta1(){
        return (delta_ - gamma_);
    }

    float lintoangle(){
      return( (Ez_/(2*M_PI*r_))*360 );
    }




    float calc_theta3(){
      //Ermittelt ueber den Kosinussatz
      return (180-radtodegree(acos(  (l04_*l04_)-(L2_*L2_)-(L1_*L1_)+(2*L2_*L1_)  )));
    }

    float calc_theta4(){
      //Ermittelt ueber den Kosinussatz
      return (180-radtodegree(acos(  (l14_*l14_)-(L1_*L1_)-(L2_*L2_)+(2*L1_*L2_)  )));
    }


    protected:
      ros::NodeHandle nh_;
      ros::Publisher pub_;
      ros::Subscriber sub_;
      std_msgs::Float64MultiArray pub_msgs_;

    //Arme:
      float L1_, L2_;
      //float L3_, L4_;
    //Base:
      float L0_;
    //Circle aprox of gear on Z-Axis
      float r_;
    //"Direkter Weg:"
      float l04_, l14_;
    //Winkel
      float alpha_, beta_, theta0_;
      float gamma_, delta_,theta1_;
      float theta2_, theta3_, theta4_;
    //Input:
      float Ex_, Ey_, Ez_;
};//End of class Inverse_Class

//-----------------------//
//--- Main processing ---//
//-----------------------//
/// Main Function
/** Responsible for argument evaluation, Subscriber Handling/Synchronization and ros::spin() */
int main(int argc, char **argv){

    ros::init(argc, argv, "inverse_kinematics");
    //Create the Object
    Inverse_Class inverese_double_scara;
    ros::spin();
    //everything is fine :)
}
