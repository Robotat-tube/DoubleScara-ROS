**********************
A simple test method
1)
roscore

2)
rosrun forward_kinematics forward_kinematics

3)
rostopic echo /checked_angles_array

4)
rostopic echo /calc_pose


5)
rostopic pub /set_angles_array std_msgs/Float64MultiArray "layout:
  dim:
  - label: ''
    size: 0
    stride: 0
  data_offset: 0
data: [0.0, 0.0, 0.0] " 



6) Check the results of the topics


