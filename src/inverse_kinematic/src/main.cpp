
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
            l04_    = calc_l04();
            alpha_  = calc_alpha();
            beta_   = calc_beta();
            theta0_ = calc_theta0();
          //Equation 3-4
            //Winkel werden noch auf die "andere seite gelegt" -> Equation 3-4 noch 180-Ergebnis
            //180-Ergebnis aufgrund der Skizze in Teams
            l14_    = calc_l14();
            gamma_  = 180 - calc_gamma();
            delta_  = 180 - calc_delta();
            theta1_ = calc_theta1();

          // Z-Axis
          // Circumference -> 2*PI*r

            theta2_ = lintoangle();


          //Simulationswinkel:
            theta3_ = calc_theta3(); 
            theta4_ = calc_theta4(); 
            

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
        return (90 - radtodegree(atan((Ey_/Ex_))));
    }
    float calc_beta(){
        return radtodegree( acos(L1_*L1_ - L2_*L2_ + l04_*l04_)/(2*L1_*l04_) );
    }
    float calc_theta0(){
        return (beta_ - alpha_);
    }

    float calc_l14(){
        return sqrt( ((L0_-Ex_)*(L0_-Ex_)) + (Ey_*Ey_));
    }
    float calc_gamma(){
        return ( 90 - radtodegree(atan(Ey_/(L0_ - Ex_))));
    }
    float calc_delta(){
        return ( radtodegree( acos( (L1_*L1_ - L2_*L2_ + l14_*l14_)/(2*L1_*l14_))));
    }
    float calc_theta1(){
        return (delta_ - gamma_);
    }

    float lintoangle(){
      return( (Ez_/(2*M_PI*r_))*360 );
    }

    float calc_theta3(){
      //Ermittelt ueber den Kosinussatz
      return (radtodegree(acos(  (l04_*l04_)-(L2_*L2_)-(L1_*L1_)+(2*L2_*L1_)  )));
    }
    
    float calc_theta4(){
      //Ermittelt ueber den Kosinussatz
      return (radtodegree(acos(  (l14_*l14_)-(L1_*L1_)-(L2_*L2_)+(2*L1_*L2_)  )));
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
