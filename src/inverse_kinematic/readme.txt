**********************
A simple test method
1)
roscore

2)
rosrun MMR_Mech_Double_Scara_Invers inverse_kinematic

3)
rostopic echo /calc_angles_array

5)
rostopic pub /set_pose geometry_msgs/Pose "position:
  x: 0.0
  y: 0.0
  z: 0.0
orientation:
  x: 0.0
  y: 0.0
  z: 0.0
  w: 0.0" 

6) check if this is the right result

